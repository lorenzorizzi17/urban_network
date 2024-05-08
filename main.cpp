#include <iostream>
#include<fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include"create_data.hpp"
#include"dual_graph.hpp"


// typedef simply creates an alias

//two classes for graphs: adjacency list and adjacency matrix. The former is optimized for sparse graphs, the latter for very dense ones 
typedef boost::adjacency_list<boost::vecS,         //stores out edges of each vertex in a std::list
                              boost::vecS,          //stores vertex set in a std::vector
                              boost::undirectedS,     //graph is undirected
                              boost::no_property,            //vertex property: VertexData
                              boost::property<boost::edge_index_t, double>   //edge property: a weight
                              > Graph;

//every vertex or edge is accessible via a opaque handle called a descriptor (vertex or edge descriptor). Let's just call them Vertex, Edge
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
//property maps simply stores keys and corresponding values. Here, every edge has a weight

typedef boost::graph_traits<Graph>::edge_iterator iter_e;

//I think every graph has some traits and type predefined, such as the type specific for vertices size
typedef boost::graph_traits<Graph>::vertices_size_type size_type;
typedef std::vector<std::pair<Vertex, Vertex>> EdgeList;
typedef std::vector<Edge> vec_t;


namespace std {
    template <typename T>
    std::istream& operator>>(std::istream& in, std::pair<T,T>& p) {
    in >> p.first >> p.second;
    return in;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out, std::pair<T,T> const& p) {
    out << p.first << " " << p.second;
    return out;
    }
}

int main() {
    //N is the dimension of the urban network (number of crosspoints)
    srand (time(NULL));
    const int N = 144;
    boost::create_manhattan_data(std::sqrt(N));
    Graph g(N);
    std::ifstream input_file("data/manhattan_grid.dat");
    Edge e;
    while(input_file.good()){
        int x,y;
        input_file >> x >> y;
        std::pair<Edge, bool> result = boost::add_edge(x, y, g);
    }
    boost::add_diagonal_roads<Graph,int>(g,3,std::sqrt(N));
    std::ofstream file("fig/graph.dot");
    boost::write_graphviz(file, g);
    file.close();
    system("dot -Tpng fig/graph.dot -o fig/graph.png");
    Graph dual = boost::make_dual_graph(g);
    std::ofstream file_dual("fig/graph_dual.dot");
    boost::write_graphviz(file_dual, dual);
    file_dual.close();
    system("dot -Tpng fig/graph_dual.dot -o fig/graph_dual.png");

}
