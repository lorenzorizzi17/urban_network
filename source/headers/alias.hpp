#ifndef ALIAS
#define ALIAS

#include</home/lorenzo17/boost/boost/graph/adjacency_list.hpp>
#include<random>

//need at least declaration
class Agent;

//create a new property tag
namespace boost {
  enum edge_agents_t { edge_agents = 111 }; // a unique #
  BOOST_INSTALL_PROPERTY(edge, agents);
}



//typedef of graph
typedef boost::adjacency_list<boost::vecS,                                  // stores out edges of each vertex in a std::list
                              boost::vecS,                                  // stores vertex set in a std::vector
                              boost::undirectedS,                           // graph is undirected
                              boost::no_property,                                   // vertex property: none
                              boost::property<boost::edge_weight_t, double,
                                   boost::property<boost::edge_agents_t, std::vector<Agent>>> // edge property: a weight and a vector of Agent
                              > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator Iter_Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator Iter_OutEdge;
typedef boost::graph_traits<Graph>::adjacency_iterator Iter_Adj;
typedef boost::property_map<Graph, boost::edge_weight_t>::type PropertyMap;
typedef boost::property_map<Graph, boost::vertex_index_t>::type IdMap;
typedef boost::property_map<Graph, boost::edge_agents_t>::type AgentMap;
typedef boost::graph_traits<Graph>::vertex_iterator Iter_Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type size_type;




#endif