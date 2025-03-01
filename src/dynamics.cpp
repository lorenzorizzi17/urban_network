//this .cpp file contains the implementation of the dynamics of the model (flow, weights, ...)
#include "headers/alias.hpp"
#include "headers/agents.hpp"
#include "headers/ODModel.hpp"
#include <omp.h>

//loss function
double f(double x) {
    return 0.001f * (x - 1) * (x - 1) * (x - 1) * (x - 1);
}

void ODModel::flow(Vertex v, int flow_rate)
{
    std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, m_dual, v);

    if (queue.empty()) {
        return;
    }

    int c = 0;
    auto it = queue.begin();

    //iterate on the queue and move the agents to the next vertex if possible (given a max flux)
    while ((it != queue.end()) && (c < flow_rate))
    {
        if ( !((*it)->has_traveled()) && get(&VertexProperty::full, m_dual, (*it)->get_next_vertex()) == false)
        {

            if(m_config.PARSING_MODE){
                PVertex old_vertex = *std::find_if(boost::vertices(m_parser).first, boost::vertices(m_parser).second, [&](PVertex v) {return m_dual[(*it)->get_vertex()].index == m_parser[v].index; });
                PVertex new_vertex = *std::find_if(boost::vertices(m_parser).first, boost::vertices(m_parser).second, [&](PVertex v) {return m_dual[(*it)->get_vertex()].index == m_parser[v].index; });
                get(boost::edge_weight, m_parser, (boost::edge(old_vertex, new_vertex, m_parser).first)) += 1;
                get(&ParserProperty::pass, m_parser, old_vertex) += 1;
            }
            
            (*it)->evolve_dijsktra();
            boost::get(&VertexProperty::queue, m_dual, (*it)->get_vertex()).push_back(std::move((*it))); 
            it = queue.erase(it);
            c++;

            if(m_config.PROCESS_STATS){
                m_stats.update_flux();
            }
        }
        else {
            it++;
        }
    }
    
}
//flow function
void ODModel::flow_multithreading(Vertex v, int flow_rate, std::vector<omp_lock_t>& locks)
{
    omp_set_lock(&locks[v]);
    std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, m_dual, v);

    if (queue.empty()) {
        omp_unset_lock(&locks[v]);
        return;
    }

    int c = 0;
    auto it = queue.begin();

    

    //iterate on the queue and move the agents to the next vertex if possible (given a max flux)
    while ((it != queue.end()) && (c < flow_rate))
    {
        if ( !((*it)->has_traveled()) && get(&VertexProperty::full, m_dual, (*it)->get_next_vertex()) == false)
        {

            if(m_config.PARSING_MODE){
                PVertex old_vertex = *std::find_if(boost::vertices(m_parser).first, boost::vertices(m_parser).second, [&](PVertex v) {return m_dual[(*it)->get_vertex()].index == m_parser[v].index; });
                PVertex new_vertex = *std::find_if(boost::vertices(m_parser).first, boost::vertices(m_parser).second, [&](PVertex v) {return m_dual[(*it)->get_vertex()].index == m_parser[v].index; });
                get(boost::edge_weight, m_parser, (boost::edge(old_vertex, new_vertex, m_parser).first)) += 1;
                get(&ParserProperty::pass, m_parser, old_vertex) += 1;
            }
            
            (*it)->evolve_dijsktra();
            int lock = (*it)->get_vertex();

            boost::get(&VertexProperty::queue, m_dual, (*it)->get_vertex()).push_back(std::move((*it))); 
            

            it = queue.erase(it);
            c++;

            if(m_config.PROCESS_STATS){
                m_stats.update_flux();
            }
        }
        else {
            it++;
        }
    }
    omp_unset_lock(&locks[v]);
    
}

//update the weghts
void ODModel::update_weights(Vertex)
{
    auto range = boost::edges(m_dual);

    for (auto it_e = range.first; it_e != range.second; it_e++) {
        int occupancy_src = (m_dual[boost::source(*it_e, m_dual)].queue).size();
        int occupancy_tgt = (m_dual[boost::target(*it_e, m_dual)].queue).size();
        get(boost::edge_weight, m_dual, *it_e) = get(&EdgeProperty::initial_weight, m_dual, *it_e) + f(occupancy_src + occupancy_tgt);
    }
}

//erase the agents that have arrived
void ODModel::erase_agents(Vertex v)
{
    std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, m_dual, v);
    auto it = std::find_if(queue.begin(), queue.end(), [](std::shared_ptr<Agent>& a_ptr){ return a_ptr->arrived(); });
    while (it != queue.end()) {
        if(m_config.PROCESS_STATS){
            m_stats.update_lifespan((*it)->m_total_time);
        }
        queue.erase(it);
        if (m_config.CONSTANT_AGENTS != 0) {
            add_agents(1);
        }
        it = std::find_if(queue.begin(), queue.end(), [](std::shared_ptr<Agent>& a_ptr){ return a_ptr->arrived(); });
    }
}


void ODModel::set_flag(Vertex v) {

    std::list<std::shared_ptr<Agent>>& queue = get(&VertexProperty::queue, m_dual, v);
    //decide whether a node is saturated 
    if (queue.size() >= m_config.MAX_CAP) {
        get(&VertexProperty::full, m_dual,v) = true;
    } else {
        get(&VertexProperty::full, m_dual, v) = false;
    }
    
	//total n. of cars that have passed through the node
    get(&VertexProperty::congestion_time, m_dual, v) += get(&VertexProperty::queue, m_dual, v).size();

    //flags regarding the agents
    bool count = true;
    if (!queue.empty()) {
        for (auto it2 = queue.begin(); it2 != queue.end(); it2++) {
            //set travel flag
            (*it2)->set_traveled(false);
            //agent total time
            (*it2)->m_total_time++;
            //agent stationary time
            (*it2)->set_perm_time((*it2)->get_perm_time() + 1);
            if ((*it2)->get_perm_time() > 500) {
                count = count && true;
            } else {
                count = count && false;
            }
        }
        //set node as completely congested if every agent has been sitting in the node for a certain amount of time 
        get(&VertexProperty::congested, m_dual, v) = count;
    }
}




/*
void print_info(Vertex v, Graph& g, Graph& dual, int time, std::map<Vertex, Edge> map_dual)
{
    std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, dual, v);
    if (queue.size() != 0)
    {
        DEBUG("v. " + std::to_string(dual[v].index) + " | agents id : ");
        std::for_each(queue.begin(), queue.end(), [&](std::shared_ptr<Agent> const& a_ptr)
            { std::cout << a_ptr->get_id() << " ( to "; if (a_ptr->arrived()) { std::cout << "arrived. "; }
            else { std::cout << dual[a_ptr->get_next_vertex()].index << "), "; };  });
        std::cout << std::endl << std::endl;
    }
}
*/