#include</home/lorenzo17/boost/boost/graph/adjacency_list.hpp>
#include<random>

typedef boost::adjacency_list<boost::vecS,                                  // stores out edges of each vertex in a std::list
                              boost::vecS,                                  // stores vertex set in a std::vector
                              boost::undirectedS,                           // graph is undirected
                              VertexData,                                   // vertex property: VertexData
                              boost::property<boost::edge_weight_t, double> // edge property: a weight
                              >
                            Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator Iter_Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator Iter_OutEdge;
typedef boost::graph_traits<Graph>::adjacency_iterator Iter_Adj;
typedef boost::property_map<Graph, boost::edge_weight_t>::type PropertyMap;
typedef boost::property_map<Graph, boost::vertex_index_t>::type IdMap;
typedef boost::graph_traits<Graph>::vertex_iterator Iter_Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type size_type;
