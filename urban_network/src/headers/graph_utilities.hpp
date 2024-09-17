#ifndef GRAPH_UTILITIES_HPP
#define GRAPH_UTILITIES_HPP

#include"alias.hpp"

//get a random vertex from the graph
inline Iter_Vertex get_random_vertex(Graph const& g) {
    static std::random_device rd;  // Seed del generatore
    static std::mt19937 gen(rd()); // Generatore Mersenne Twister
    std::uniform_int_distribution<> dis; // Distribuzione uniforme

    Iter_Vertex vi, vi_end;
    std::tie(vi, vi_end) = boost::vertices(g);
    int num_e = boost::num_vertices(g);
    dis.param(std::uniform_int_distribution<>::param_type(0, num_e - 1));

    int c = 0;
    while (c < 1000) {
        Iter_Vertex vi = boost::vertices(g).first;
        std::advance(vi, dis(gen));
        if (get(&VertexProperty::queue, g, *vi).size() < 150) {
            return vi;
        }
        c++;
    }
    throw std::runtime_error("Unable to locate new agents");
}

inline Iter_Edge get_random_edge(Graph const& g) {
    Iter_Edge ei = boost::edges(g).first;
    int num_e = boost::num_edges(g);
    std::advance(ei, std::rand() % num_e);
    return ei;
}

#endif