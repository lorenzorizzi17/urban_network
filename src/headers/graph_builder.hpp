#ifndef GRAPH_BUILDER
#define GRAPH_BUILDER

#include <boost/graph/graphviz.hpp>
#include "config.hpp"
#include "alias.hpp"
#include <cstdlib>

std::string check(std::pair<int, int> const&, std::vector<std::pair<int, int>>&, bool);

void create_manhattan_data(const int);

void add_diagonal_roads(Graph&, int, int);

void remove_random_edge(Graph&, int, int);

bool are_connected(int, int, Graph);

Graph make_dual_graph(Graph& g, std::map<Vertex, Edge>& map_dual);

void randomize_weight_map_uniform(Graph&, double, double);

void set_weights(Graph&);

Graph build_graph(Config const&);

void load_graph_dual(const std::string&, Graph&, Graph&, std::map<Vertex, Edge>&);

void load_graph(const std::string&, Graph&);

void print_graph(Graph const&, Graph const&, std::map<Vertex, Edge> const&);

#endif