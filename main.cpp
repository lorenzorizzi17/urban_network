#include <iostream>
#include<fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include"create_data.hpp"
#include"dual_graph.hpp"
#include<boost/graph/dijkstra_shortest_paths.hpp>


// typedef simply creates an alias

//two classes for graphs: adjacency list and adjacency matrix. The former is optimized for sparse graphs, the latter for very dense ones 

//every vertex or edge is accessible via a opaque handle called a descriptor (vertex or edge descriptor). Let's just call them Vertex, Edge
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
//property maps simply stores keys and corresponding values. Here, every edge has a weight
//I think every graph has some traits and type predefined, such as the type specific for vertices size
typedef boost::graph_traits<Graph>::vertices_size_type size_type;
typedef boost::property_map<Graph, boost::vertex_index_t>::type IdMap;


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
    const int N = 100;
    boost::create_manhattan_data(std::sqrt(N));
    Graph g(N);
    std::ifstream input_file("data/manhattan_grid.dat");
    Edge e;
    while(input_file.good()){
        int x,y;
        input_file >> x >> y;
        std::pair<Edge, bool> result = boost::add_edge(x, y, g);
    }
    boost::add_diagonal_roads<Graph,int>(g,10,std::sqrt(N));
    boost::remove_random_edge<Graph,Iter_Edge>(g,5,std::sqrt(N));
    std::ofstream file("fig/graph.dot");
    boost::write_graphviz(file, g);
    file.close();
    system("neato -Tpng fig/graph.dot -o fig/graph.png");

    Graph dual = boost::make_dual_graph(g);
    std::ofstream file_dual("fig/graph_dual.dot");
    boost::write_graphviz(file_dual, dual);
    file_dual.close();
    system("fdp -Tpng fig/graph_dual.dot -o fig/graph_dual.png");

    boost::randomize_weight_map_uniform<Graph, Iter_Edge>(g,5,7);
    boost::print_weight_map<Graph, Iter_Edge>(g);

    
    PropertyMap weightmap = get(boost::edge_weight, g);
    std::vector<Vertex> pred(num_vertices(g));  // int this vector, the algorithm will store the predecessor
    std::vector<int> dist_vector(num_vertices(g));
    boost::iterator_property_map<std::vector<Vertex>::iterator, IdMap, Vertex, Vertex&> predmap(pred.begin(), boost::get(boost::vertex_index,g));
    
    Vertex s = *(boost::vertices(g).first);
    boost::property_map<Graph, boost::vertex_index_t>::type indexmap = get(boost::vertex_index, g);
    boost::iterator_property_map<std::vector<int>::iterator, IdMap, int, int&> distmap_vec(dist_vector.begin(), boost::get(boost::vertex_index,g));
    boost::dijkstra_shortest_paths(g, s, boost::predecessor_map(predmap).distance_map(distmap_vec));
    for (auto it = pred.begin(); it != pred.end(); it++)
    {
        std::cout << *it << std::endl;
    }
    
}
