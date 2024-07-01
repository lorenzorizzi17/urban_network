#include "./alias.hpp"

namespace boost {


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
}