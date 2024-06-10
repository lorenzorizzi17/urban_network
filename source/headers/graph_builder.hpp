#include <iostream>
#include <fstream>
#include"randomize.hpp"
#include <boost/graph/graphviz.hpp>
#include "create_data.hpp"
#include "dual_graph.hpp"
#include "alias.hpp"

Graph build_graph(int const N){
    srand(time(NULL));

    // creating manhattan data
    boost::create_manhattan_data(std::sqrt(N));

    // storing the graph based on previous operation on .txt file
    Graph g(N);
    std::ifstream input_file("data/manhattan_grid.dat");
    while (input_file.good())
    {
        int x, y;
        input_file >> x >> y;
        std::pair<Edge, bool> result = boost::add_edge(x, y, g);
    }

    // add a little bit of randomization
    boost::add_diagonal_roads<Graph, int>(g, 0, std::sqrt(N));
    boost::remove_random_edge<Graph, Iter_Edge>(g, 0, std::sqrt(N));
    // randomize edges weight
    boost::randomize_weight_map_uniform<Graph, Iter_Edge>(g, 5, 7); // as for now, uniform distribution. Maybe gaussian?
    // boost::print_weight_map<Graph, Iter_Edge>(g);
    return g;
}



void print_graph(Graph const& g, Graph const& dual){
    std::ofstream file("fig/graph.dot");
    boost::write_graphviz(file, g);
    file.close();
    system("neato -Tpng fig/graph.dot -o fig/graph.png");
    std::ofstream file_dual("fig/graph_dual.dot");
    boost::write_graphviz(file_dual, dual);
    file_dual.close();
    system("fdp -Tpng fig/graph_dual.dot -o fig/graph_dual.png");
}