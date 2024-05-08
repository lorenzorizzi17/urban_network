#include <iostream>
#include<fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include"create_dual_graph.hpp"


// typedef simply creates an alias

struct VertexData{
    std::string name;
    int s;
};
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

//two classes for graphs: adjacency list and adjacency matrix. The former is optimized for sparse graphs, the latter for very dense ones 
typedef boost::adjacency_list<boost::vecS,         //stores out edges of each vertex in a std::list
                              boost::vecS,          //stores vertex set in a std::vector
                              boost::undirectedS,     //graph is undirected
                              VertexData,            //vertex property: VertexData
                              boost::property<boost::edge_index_t, double>   //edge property: a weight
                              > Graph;

int main(){
    Graph g(5);
    g[0].name = "Jon";

    //now add some edges.
    //add_edge actually returns a std::pair: a pointer to the edge and a flag
    Edge e = boost::add_edge(1,2,g).first;

    //property map:
    boost::property_map<Graph, boost::edge_weight_t>::type weightmap = get(boost::edge_weight,g);
    weightmap[e] = 10.2;

    //creating a second graph, this time filling it from a .dat file
    /* std::ifstream file_in("data/manhattan_grid.dat");
    size_type n_vertices;
    file_in >> n_vertices; // read in number of vertices

    std::istream_iterator<std::pair<size_type, size_type>> input_begin(file_in), input_end;
    Graph g2(input_begin, input_end, n_vertices);
 */
//lets perform a topo DFS topological ordering
    /* std::cout << "\nPerforming topological sorting based on DFS search:\n";
    std::vector<Vertex> ordered_list;
    boost::topological_sort(g2, std::back_inserter(ordered_list));
    for (auto it = ordered_list.begin(); it != ordered_list.end(); ++it){
        std::cout << *it << " ";
        std::cout << std::endl;
    } */

}