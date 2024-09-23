#ifndef PARSER_HPP
#define PARSER_HPP

#include "alias.hpp"

struct ParserProperty {
    int index;
    int pass;
};

typedef boost::adjacency_list<boost::vecS,                                  // stores out edges of each vertex in a std::list
    boost::vecS,                                  // stores vertex set in a std::vector
    boost::bidirectionalS,                             // graph is directed
    ParserProperty, // vertex property: a list of agents heap allocated
    boost::property<boost::edge_weight_t, double> // edge property: a weight
> Parser;

typedef boost::graph_traits<Parser>::vertex_descriptor PVertex;
typedef boost::graph_traits<Parser>::edge_descriptor PEdge;


Parser make_parser(Graph const& g);

#endif