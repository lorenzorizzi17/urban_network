#ifndef FLOW
#define FLOW

#include "./alias.hpp"
#include "./agents.hpp"
#include <algorithm>
#include <vector>

namespace boost
{
    void reactivate_flag_v(Vertex v, Graph& g){
        std::list<std::shared_ptr<Agent>> ref = boost::get(boost::vertex_agents,g,v);
        std::for_each(ref.begin(), ref.end(),[](std::shared_ptr<Agent>& ptr_a){ptr_a->set_traveled(false);});
    }

    void reactivate_flag(Graph& g){
        std::for_each(boost::vertices(g).first, boost::vertices(g).second,[&](Vertex v){boost::reactivate_flag_v(v,g);});
    }

    void flow(Vertex v, Graph &g_dual, int MAX_FLOW)
    {
        std::list<std::shared_ptr<Agent>> &queue = boost::get(boost::vertex_agents, g_dual, v);
        for (int c = 0; c < MAX_FLOW; c++)
        {
            if ((queue.size() != 0))
            {
                if (!queue.front()->has_traveled())
                {
                    queue.front()->evolve_dijsktra();
                    std::shared_ptr<Agent> new_ptr = queue.front();
                    boost::get(boost::vertex_agents, g_dual, queue.front()->get_vertex()).push_back(new_ptr);    
           
                    queue.front() = nullptr;
                    queue.pop_front();
                }
                
            }
        }
    }


    void erase_agents(Vertex v, Graph& g_dual){
        std::list<std::shared_ptr<Agent>> &queue = boost::get(boost::vertex_agents, g_dual, v);
        queue.remove_if([](std::shared_ptr<Agent> a_ptr){return a_ptr->arrived();});
    }


    void print_info(Vertex v, Graph& g, int time, std::map<Vertex,Edge> map_dual){
        std::list<std::shared_ptr<Agent>> &queue = boost::get(boost::vertex_agents, g, v);
        std::cout << "At time t = " << time << ", in road " << map_dual.at(v) << " there are agent id: " ;
        std::for_each(queue.begin(),queue.end(), [](std::shared_ptr<Agent> const& a_ptr){std::cout << a_ptr->get_id() << " ";});  
        std::cout << std::endl;
    }
}

#endif