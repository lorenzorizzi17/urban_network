#ifndef FLOW
#define FLOW

#include "./alias.hpp"
#include "./agents.hpp"
#include <algorithm>
#include <vector>

namespace boost
{
    void reactivate_flag_v(Vertex v, Graph &g)
    {
        std::list<std::shared_ptr<Agent>> ref = boost::get(&VertexProperty::queue, g, v);
        if (!ref.empty()) {
            std::for_each(ref.begin(), ref.end(), [](std::shared_ptr<Agent>& ptr_a)
                { ptr_a->set_traveled(false); });
        }
    }

    void reactivate_flag(Graph &g)
    {
        std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v)
                      { boost::reactivate_flag_v(v, g); });
    }

    void increase_perm_time(Graph &g){
        std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v)
                      { std::list<std::shared_ptr<Agent>> &ref = boost::get(&VertexProperty::queue, g, v);
                        std::for_each(ref.begin(), ref.end(), [](std::shared_ptr<Agent> &ptr_a)
                                      { ptr_a->increase_permanence_time(); });
                      });
    }


    void thermal_shake(Graph &g){
        std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v)
                      { std::list<std::shared_ptr<Agent>> &ref = boost::get(&VertexProperty::queue, g, v);
                        std::for_each(ref.begin(), ref.end(), [](std::shared_ptr<Agent> &ptr_a)
                                      { if(ptr_a->get_perm_time()>WAITING_TIME){ptr_a->thermal_move();} });
                      });
    }


    void flow(Vertex &v, Graph &g_dual, int flow_rate)
    {
        std::list<std::shared_ptr<Agent>> &queue = boost::get(&VertexProperty::queue, g_dual, v);
        if (queue.empty()) {
            return;
        }

        int c = 0;
        auto it = queue.begin(); 
        while((it != queue.end())&& (c<flow_rate))
        {
            if (!((*it)->has_traveled()) && get(&VertexProperty::queue, g_dual, (*it)->get_next_vertex()).size() <= MAX_CAP)
            {
                    (*it)->evolve_dijsktra();
                    boost::get(&VertexProperty::queue, g_dual, (*it)->get_vertex()).push_back(std::move((*it)));
                    it = queue.erase(it);
                    c++;
            } else {
                it++;
            }
        }
    }

    void erase_agents(Vertex &v, Graph &g_dual, std::map<Vertex, Edge> map)
    {
        std::list<std::shared_ptr<Agent>> &queue = boost::get(&VertexProperty::queue, g_dual, v);
        auto it = std::find_if(queue.begin(), queue.end(), [](std::shared_ptr<Agent> &a_ptr)
                     { return a_ptr->arrived(); });
        while(it != queue.end()){
            queue.erase(it);
            boost::add_agents(g_dual, 1,map);
            it = std::find_if(queue.begin(), queue.end(), [](std::shared_ptr<Agent> &a_ptr)
                     { return a_ptr->arrived(); });
        }
    }

    void print_info(Vertex v, Graph &g, Graph& dual, int time, std::map<Vertex, Edge> map_dual)
    {
        std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, dual, v);
        if (queue.size() != 0)
        {
            DEBUG( "At time t = " + std::to_string(time) + ", in road(" + std::to_string(g[boost::source(map_dual.at(v), g)].index) + " - " + std::to_string(g[boost::target(map_dual.at(v),g)].index) + ")" + "(v." + std::to_string(dual[v].index)+ "), live agents id : ");
            std::for_each(queue.begin(), queue.end(), [&](std::shared_ptr<Agent> const &a_ptr)
                          { std::cerr << a_ptr->get_id() << " ( to " << dual[a_ptr->get_next_vertex()].index << "), "; });
            std::cerr << std::endl;
        }
    }
}

#endif