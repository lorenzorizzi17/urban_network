#include<fstream>

#include"alias.hpp"

void update_queue_size(std::ofstream& file, Graph& g, int time, int ix) {
        auto it = std::find_if(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {return g[v].index == ix; });
        if (it == boost::vertices(g).second) { throw std::runtime_error{ "Error" }; };
        double partial_mean = double(g[*it].congestion_time) / double(time);
        file << partial_mean;
        file << std::endl;
}

void update_queue_size(std::ofstream& file, Graph& g, int time) {
    for (int i = 0; i < boost::num_vertices(g); i++) {
        auto it = std::find_if(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {return g[v].index == i; });
        if (it == boost::vertices(g).second) { throw std::runtime_error{ "Error" }; };
        double partial_mean = double(g[*it].congestion_time) / double(time);
        file << partial_mean << " ";
    }
    file << std::endl;
}