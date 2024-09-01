#include <boost/Graph/adjacency_list.hpp>
#include <boost/Graph/depth_first_search.hpp>
#include <iostream>
#include <fstream>
#include<math.h>
#include <vector>
#include "graph_builder.hpp"
#include"alias.hpp"

// Definizione del grafo usando BGL
struct PercolationVertexProperties {
    bool occupied; // Proprietà boolean per indicare se il vertice è "occupato"
    int index;
    double congested_time;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, PercolationVertexProperties> PercolationGraph;
typedef boost::graph_traits<PercolationGraph>::adjacency_iterator AdjacencyIterator;

void dfs_cluster(PercolationGraph& g, Vertex v, std::vector<bool>& visited, std::vector<Vertex>& cluster) {
    visited[v] = true;
    cluster.push_back(v);

    // Ottieni gli adiacenti
    std::pair<AdjacencyIterator, AdjacencyIterator> neighbors = adjacent_vertices(v, g);
    for (AdjacencyIterator it = neighbors.first; it != neighbors.second; ++it) {
        if (g[*it].occupied && !visited[*it]) {
            dfs_cluster(g, *it, visited, cluster);
        }
    }
}

std::vector<std::vector<Vertex>> find_clusters(PercolationGraph& g) {
    std::vector<std::vector<Vertex>> clusters;
    std::vector<bool> visited(num_vertices(g), false);

    std::pair<Iter_Vertex, Iter_Vertex> vertices = boost::vertices(g);
    for (Iter_Vertex it = vertices.first; it != vertices.second; ++it) {
        if (g[*it].occupied && !visited[*it]) {
            std::vector<Vertex> cluster;
            dfs_cluster(g, *it, visited, cluster);
            clusters.push_back(cluster);
        }
    }

    return clusters;
}


void load_graph(const std::string& filename, PercolationGraph& PercolationGraph) {
    std::ifstream dot_file(filename);

    boost::dynamic_properties dp(boost::ignore_other_properties);

    auto index_map_vert = get(&PercolationVertexProperties::index, PercolationGraph);
    dp.property("label", index_map_vert);
    dp.property("occupied", boost::get(&PercolationVertexProperties::occupied, PercolationGraph));

    if (!boost::read_graphviz(dot_file, PercolationGraph, dp)) {
        std::cerr << "Error reading graphviz file: " << filename << std::endl;
    }

}


std::pair<size_t, size_t> find_largest_and_second_largest_cluster_length(const std::vector<std::vector<Vertex>>& clusters) {
    size_t max_length = 0;
    size_t second_max_length = 0;

    for (const auto& cluster : clusters) {
        size_t cluster_size = cluster.size();

        if (cluster_size > max_length) {
            // Sposta la lunghezza massima attuale nella seconda lunghezza massima
            second_max_length = max_length;
            // Aggiorna la lunghezza massima
            max_length = cluster_size;
        }
        else if (cluster_size > second_max_length && cluster_size < max_length) {
            // Aggiorna la seconda lunghezza massima se è minore della lunghezza massima ma maggiore della seconda
            second_max_length = cluster_size;
        }
    }

    return { max_length, second_max_length };
}

void percolation(int N_AGENTS, int THRESHOLD ){
    int p = THRESHOLD;
    PercolationGraph g;
     std::cout << std::to_string(int(std::round(p))) ;
        load_graph("fig/graph_percolation_N"+std::to_string(N_AGENTS) + "_p" + std::to_string(p) + ".txt", g);

        std::vector<std::vector<Vertex>> clusters = find_clusters(g);

        // Stampa i cluster
        for (size_t i = 0; i < clusters.size(); ++i) {
            std::cout << "Cluster " << i + 1 << ": ";
            for (Vertex v : clusters[i]) {
                std::cout << g[v].index << " ";
            }
            std::cout << std::endl;
        }
        std::ofstream file("fig/perc/S/S_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
        std::ofstream file_sc("fig/perc/SecondCluster/second_cluster_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
        file << double(p/100.0) << " " << find_largest_and_second_largest_cluster_length(clusters).first << std::endl;
        file_sc << double(p/100.0) << " " << find_largest_and_second_largest_cluster_length(clusters).second << std::endl;
    }
    
