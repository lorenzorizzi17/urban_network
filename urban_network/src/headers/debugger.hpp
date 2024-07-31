#ifndef DEBUGGER
#define DEBUGGER

#include "./alias.hpp"
#include"gridlock_ecception.hpp"


namespace od {
    //create an agent whose spawn and destination point are already known at creation time
    void add_agents(Graph& g_dual, Vertex vi, Vertex vf, std::map<Vertex, Edge>& dual_map)
    {
        boost::get(&VertexProperty::queue, g_dual, vi).push_back(std::make_shared<Agent>(g_dual, vi, vf, dual_map));
    }

    //create a stationary agent
    void add_stationary_agents(Graph& g_dual, Vertex vi, Vertex vf, std::map<Vertex, Edge>& dual_map, int n_debug)
    {
        boost::get(&VertexProperty::queue, g_dual, vi).push_back(std::make_shared<Agent>(g_dual, vi, vf, dual_map, n_debug));
    }

    int d_check_sum(Graph const& g) {
        int sum = 0;
        for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
            sum += get(&VertexProperty::queue, g, *it).size();
        }
        return sum;
    }


    void print_queue(Graph const& g){
        for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
            std::cout << "Vertex v: " << g[*it].index << " occ: " << get(&VertexProperty::queue, g, *it).size() << std::endl;
        }
    }


    void d_print_info(Graph& g, std::ofstream& file, int time) {
        file << " #### TIME = " << time << std::endl;
        for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
            std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, g, *it);
            if (!queue.empty()) {
                file << " vertex" << g[*it].index << ": ";
                std::for_each(queue.begin(), queue.end(), [&](std::shared_ptr<Agent> const& a_ptr)
                    { file << a_ptr->get_id() << " ( to "; if (a_ptr->arrived()) { std::cout << "arrived. "; }
                    else { file << g[a_ptr->get_next_vertex()].index << "), "; };  });
                file << std::endl << std::endl;
            }
            
        }
    }


   
}

#endif