#ifndef FLOW
#define FLOW

#include "./alias.hpp"
#include "./agents.hpp"
#include "stats.hpp"
#include <algorithm>
#include <vector>

namespace od
{
    void reactivate_flag_v(Vertex v, Graph& g)
    {
        std::list<std::shared_ptr<Agent>> ref = boost::get(&VertexProperty::queue, g, v);
        if (!ref.empty()) {
            std::for_each(ref.begin(), ref.end(), [](std::shared_ptr<Agent>& ptr_a)
                { ptr_a->set_traveled(false); ptr_a->set_perm_time(ptr_a->get_perm_time() + 1); ptr_a->m_total_time++; });
        }
    }

    void reactivate_flag(Graph& g)
    {
        std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v)
            { reactivate_flag_v(v, g); });
    }


    void flow(Vertex& v, Graph& g_dual, int flow_rate, Parser& p, Statistics& stat)
    {
        std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, g_dual, v);
        if (queue.empty()) {
            return;
        }

        PVertex pv = *std::find_if(boost::vertices(p).first, boost::vertices(p).second, [&](PVertex pv) {return p[pv].index == g_dual[v].index; });

        int c = 0;
        auto it = queue.begin();
        while ((it != queue.end()) && (c < flow_rate))
        {
            if (!((*it)->has_traveled()) && get(&VertexProperty::full, g_dual, (*it)->get_next_vertex()) == false)
            {
                PVertex old_vertex = *std::find_if(boost::vertices(p).first, boost::vertices(p).second, [&](PVertex v) {return g_dual[(*it)->get_vertex()].index == p[v].index; });
                (*it)->evolve_dijsktra();
                PVertex new_vertex = *std::find_if(boost::vertices(p).first, boost::vertices(p).second, [&](PVertex v) {return g_dual[(*it)->get_vertex()].index == p[v].index; });
                if (new_vertex == old_vertex) { throw std::runtime_error("Same vertex"); }
                get(boost::edge_weight, p, (boost::edge(pv, new_vertex, p).first)) += 1;
                get(&ParserProperty::pass, p, pv) += 1;

                boost::get(&VertexProperty::queue, g_dual, (*it)->get_vertex()).push_back(std::move((*it)));
                it = queue.erase(it);
                c++;
                stat.update_flux();
            }
            else {
                    it++;
            }
        }

    }

    void erase_agents(Vertex& v, Graph& g_dual, std::map<Vertex, Edge> map, Statistics& s)
    {
        std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, g_dual, v);
        auto it = std::find_if(queue.begin(), queue.end(), [](std::shared_ptr<Agent>& a_ptr)
            { return a_ptr->arrived(); });
        while (it != queue.end()) {
            s.update_lifespan((*it)->m_total_time);
            queue.erase(it);
            add_agents(g_dual, 1, map);
            it = std::find_if(queue.begin(), queue.end(), [](std::shared_ptr<Agent>& a_ptr)
                { return a_ptr->arrived(); });
        }
    }

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



}

#endif