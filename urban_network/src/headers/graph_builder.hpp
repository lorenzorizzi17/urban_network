#include <iostream>
#include <fstream>
#include"randomize.hpp"
#include <boost/graph/graphviz.hpp>
#include "create_data.hpp"
#include "dual_graph.hpp"
#include "alias.hpp"
#include <cstdlib>
#include <boost/property_map/dynamic_property_map.hpp>
#include <libs/graph/src/read_graphviz_new.cpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

Graph build_graph(int const N){
    srand(time(NULL));

    // creating manhattan data
    boost::create_manhattan_data(std::sqrt(N));

    // storing the graph based on previous operation on .txt file
    Graph g(N);
    for(int i = 0; i < N; i++){
        get(&VertexProperty::index, g)[i] = i;
    }
    std::ifstream input_file("data/manhattan_grid.dat");
    while (input_file.good())
    {
        int x, y;
        input_file >> x >> y;
        std::pair<Edge, bool> result = boost::add_edge(x, y, g);
    }

    // add a little bit of randomization
    boost::add_diagonal_roads(g, N_DIAGONAL_ROADS, std::sqrt(N));
    boost::remove_random_edge(g, N_REMOVED_ROADS, std::sqrt(N));
    int c = 0;
    for(auto it = boost::edges(g).first; it != boost::edges(g).second; it++,c++){
        get(&EdgeProperty::index,g,*it) = c;
    }
    // randomize edges weight
    boost::randomize_weight_map_uniform(g, 5, 7); // as for now, uniform distribution. Maybe gaussian?
    // boost::print_weight_map<Graph, Iter_Edge>(g);
    return g;
}


void load_graph_dual(const std::string& filename, Graph& dual, Graph& graph, std::map<Vertex, Edge>& dual_map) {
    std::ifstream dot_file(filename);

    boost::dynamic_properties dp(boost::ignore_other_properties);
    auto index_map = get(&VertexProperty::index, dual);
    auto weight_map = get(boost::edge_weight, dual);

    dp.property("label", index_map);
    dp.property("weight", weight_map);

    if (!boost::read_graphviz(dot_file, dual, dp)) {
        std::cerr << "Error reading graphviz file: " << filename << std::endl;
    }

    for(auto it = boost::vertices(dual).first; it != boost::vertices(dual).second; it++){
        int index = get(&VertexProperty::index, dual, *it);
        auto edg = std::find_if(boost::edges(graph).first, boost::edges(graph).second, [&](Edge e){
            return (get(&EdgeProperty::index, graph, e) == index);});
       dual_map[*it] = *edg;
    }
}

void load_graph(const std::string& filename, Graph& graph) {
    std::ifstream dot_file(filename);

    boost::dynamic_properties dp(boost::ignore_other_properties);
    
    auto index_map_vert = get(&VertexProperty::index, graph);
    auto index_map_edge = get(&EdgeProperty::index, graph);
    auto weight_map = get(boost::edge_weight, graph);
    dp.property("label", index_map_vert);
    dp.property("label_edge", index_map_edge);
    dp.property("weight", weight_map);

    if (!boost::read_graphviz(dot_file, graph, dp)) {
        std::cerr << "Error reading graphviz file: " << filename << std::endl;
    }

}

void print_graph(Graph const& g, Graph const& dual, std::map<Vertex, Edge> const& dual_map){
    std::ofstream file("fig/graph.dot");
    auto edge_writer = [&](std::ostream& out, const Edge e) {
        out << "[weight=\"" << boost::get(boost::edge_weight, g, e) << "\", label_edge= \"" << boost::get(&EdgeProperty::index, g, e) << "\"]";
    };

    auto edge_writer_dual = [&](std::ostream& out, const Edge e) {
        out << "[weight=\"" << boost::get(boost::edge_weight, dual, e) << "\"]";
    };

    auto vertex_writer = [&](std::ostream& out, const auto& v) {
        out << "[label=\"" << boost::get(&VertexProperty::index, g,v) << "\"]";
    };

    auto vertex_writer_dual = [&](std::ostream& out, const auto& v) {
        out << "[label=\"" << boost::get(&EdgeProperty::index, g, dual_map.at(v)) << "\"]";
    };
    
    boost::write_graphviz(file, g, vertex_writer, edge_writer);
    file.close();
    std::system("neato -Tpng fig/graph.dot -o fig/graph.png");
    std::ofstream file_dual("fig/graph_dual.dot");
    boost::write_graphviz(file_dual, dual, vertex_writer_dual, edge_writer_dual);
    file_dual.close();
    std::system("fdp -Tpng fig/graph_dual.dot -o fig/graph_dual.png");
}

std::string intToColor(int i, int MAX) {
    // Ensure i is within the range [0, MAX]
    i = std::max(0, std::min(i, MAX));

    // RGB values for blue (0, 0, 255) and red (255, 0, 0)
    int blueR = 0, blueG = 0, blueB = 255;
    int redR = 255, redG = 0, redB = 0;

    // Calculate the interpolation factor
    double t = static_cast<double>(i) / MAX;

    // Interpolate between blue and red
    int R = static_cast<int>((1 - t) * blueR + t * redR);
    int G = static_cast<int>((1 - t) * blueG + t * redG);
    int B = static_cast<int>((1 - t) * blueB + t * redB);

    // Convert RGB values to hexadecimal string
    std::stringstream ss;
    ss << "#" << std::setfill('0') << std::setw(2) << std::hex << R
        << std::setfill('0') << std::setw(2) << std::hex << G
        << std::setfill('0') << std::setw(2) << std::hex << B;

    return ss.str();
}


void print_dual_paint(Graph const& dual) {
    std::ofstream file_dual("fig/graph_dual_paint.dot");
    auto edge_writer_dual = [&](std::ostream& out, const Edge e) {
        out << "[weight=\"" << boost::get(boost::edge_weight, dual, e) << "\"]";
        };

    auto vertex_writer_dual = [&](std::ostream& out, const auto& v) {
        out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"" << intToColor((dual[v].queue).size(), MAX_CAP+1) << "\", style = \"filled\" ]";
        };

    boost::write_graphviz(file_dual, dual, vertex_writer_dual, edge_writer_dual);
    file_dual.close();
    std::system("fdp -Tpng fig/graph_dual_paint.dot -o fig/graph_dual_paint.png");
   
    
}