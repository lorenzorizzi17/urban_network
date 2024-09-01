#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <iostream>
#include<fstream>
#include <vector>

#include "alias.hpp"

typedef boost::graph_traits<Graph>::adjacency_iterator AdjacencyIterator;

void dfs_cluster(Graph& g, Vertex v, std::vector<bool>& visited, std::vector<Vertex>& cluster) {
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

std::vector<std::vector<Vertex>> find_clusters(Graph& g) {
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

