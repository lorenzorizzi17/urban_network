#ifndef ALIAS
#define ALIAS

#include<./boost/graph/adjacency_list.hpp>
#include<random>
#include<memory>

//need at least declaration
class Agent;

//create a new property tag
namespace boost {
  enum vertex_agents_t { vertex_agents = 111 }; // a unique #
  BOOST_INSTALL_PROPERTY(vertex, agents);
}



//typedef of graph
typedef boost::adjacency_list<boost::vecS,                                  // stores out edges of each vertex in a std::list
                              boost::vecS,                                  // stores vertex set in a std::vector
                              boost::directedS,                             // graph is directed
                              boost::property<boost::vertex_agents_t, std::list<std::shared_ptr<Agent>>>, // vertex property: a list of agents heap allocated
                              boost::property<boost::edge_weight_t, double> // edge property: a weight 
                              > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator Iter_Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator Iter_OutEdge;
typedef boost::graph_traits<Graph>::adjacency_iterator Iter_Adj;
typedef boost::property_map<Graph, boost::edge_weight_t>::type PropertyMap;
typedef boost::property_map<Graph, boost::vertex_index_t>::type IdMap;
typedef boost::property_map<Graph, boost::vertex_agents_t>::type AgentMap;
typedef boost::graph_traits<Graph>::vertex_iterator Iter_Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type size_type;



void DEBUG(std::string const& s){
  #ifdef DEBUG_MODE
    std::cout << "-- DEBUG LOG --" << s << std::endl;
  #endif
}
#endif