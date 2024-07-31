#ifndef GRAPH_BUILDER
#define GRAPH_BUILDER

#include <iostream>
#include <fstream>
#include <boost/graph/graphviz.hpp>
#include "alias.hpp"
#include <cstdlib>
#include <boost/property_map/dynamic_property_map.hpp>
#include <libs/graph/src/read_graphviz_new.cpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>



void load_graph(const std::string& filename, Graph& graph) {
    std::ifstream dot_file(filename);

    boost::dynamic_properties dp(boost::ignore_other_properties);

    auto index_map_vert = get(&VertexProperty::index, graph);
    auto index_map_edge = get(&EdgeProperty::index, graph);
    dp.property("label", index_map_vert);
    dp.property("label_edge", index_map_edge);

    if (!boost::read_graphviz(dot_file, graph, dp)) {
        std::cerr << "Error reading graphviz file: " << filename << std::endl;
    }

}


void load_dual(const std::string& filename, Graph& parser){
    std::ifstream dot_file(filename);

    boost::dynamic_properties dp(boost::ignore_other_properties);

    auto index_map_vert = get(&VertexProperty::index, parser);
    auto index_map_edge = get(&EdgeProperty::prob, parser);
    dp.property("label", index_map_vert);
    dp.property("probability", index_map_edge);
    if (!boost::read_graphviz(dot_file, parser, dp)) {
        std::cerr << "Error reading graphviz file: " << filename << std::endl;
    }
}


#endif