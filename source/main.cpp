#include <iostream>
#include<fstream>
#include<boost/graph/graphviz.hpp>
#include"./headers/create_data.hpp"
#include"./headers/dual_graph.hpp"
#include"./headers/alias.hpp"
#include"./headers/dijkstra.hpp"
#include"./headers/agents.hpp"

namespace std{
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
};

int main() {
    //N is the dimension of the urban network (number of crosspoints)
    srand (time(NULL));
    const int N = 81;
    boost::create_manhattan_data(std::sqrt(N));
    Graph g(N);
    std::ifstream input_file("data/manhattan_grid.dat");
    Edge e;
    while(input_file.good()){
        int x,y;
        input_file >> x >> y;
        std::pair<Edge, bool> result = boost::add_edge(x, y, g);
    }
    boost::add_diagonal_roads<Graph,int>(g,8,std::sqrt(N));
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

    boost::randomize_weight_map_uniform<Graph, Iter_Edge>(g,5,7);    //as for now, uniform distribution. Maybe gaussian? 
    boost::print_weight_map<Graph, Iter_Edge>(g);
    
    PropertyMap weightmap = get(boost::edge_weight, g);
    std::vector<Vertex> pred(num_vertices(g));  // int this vector, the algorithm will store the predecessor
    std::vector<int> dist_vector(num_vertices(g));
    boost::iterator_property_map<std::vector<Vertex>::iterator, IdMap, Vertex, Vertex&> predmap(pred.begin(), boost::get(boost::vertex_index,g));
    
    // WIP
    /*  Vertex s = *(boost::vertices(g).first);
    auto m = (boost::vertices(g).first);
    std::advance(m,10);
    boost::property_map<Graph, boost::vertex_index_t>::type indexmap = get(boost::vertex_index, g);
    boost::iterator_property_map<std::vector<int>::iterator, IdMap, int, int&> distmap_vec(dist_vector.begin(), boost::get(boost::vertex_index,g));
    djikstra_name_printer vis(indexmap); */ 
    //MODELLO OD: pesca uniforme o pesca ai bordi. Vincoli su distanza origine e destinazione 
    //CALCOLA DIJSKTRA

    //a property map that associates graph vertices with property::vertex_index
    boost::property_map<Graph, boost::vertex_index_t>::type map_indices = boost::get(boost::vertex_index,g);
    //creating an instance of the visitor 
    bfs_index_printer<boost::property_map<Graph, boost::vertex_index_t>::type> vis(map_indices);
    //This is to store the predecessor
    std::vector<Vertex> p(num_vertices(g));
    //This is to store the distances, I guess
    std::vector<double> d(num_vertices(g));
    //calling the method BFS with vis as a visitor
    //boost::breadth_first_search(g,*boost::vertices(g).first,boost::visitor(vis));
    boost::dijkstra_shortest_paths(g, *boost::vertices(g).first,
        predecessor_map(boost::make_iterator_property_map(
                            p.begin(), get(boost::vertex_index, g)))
            .distance_map(boost::make_iterator_property_map(
                d.begin(), get(boost::vertex_index, g))));
    Iter_Vertex vi, vend;
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi)
    {
        std::cout << "distance(" << *vi << ") = " << d[boost::get(map_indices, *vi)] << ", ";
        std::cout << "parent(" << *vi << ") = " << p[boost::get(map_indices, *vi)] << std::endl;
    }
    std::cout << std::endl;

    //boost::property_map<Graph, boost::edge_weight_t>::type map_weights = boost::get(boost::edge_weight,g);
    //creating an instance of the visitor 
    /* djk_index_printer<boost::property_map<Graph,boost::edge_weight_t>::type, 
                      boost::property_map<Graph, boost::vertex_index_t>::type>  vis_d(map_weights,map_indices);
    boost::dijkstra_shortest_paths(g,*boost::vertices(g).first,boost::visitor(vis_d)); */
    //boost::breadth_first_search(g, s,boost::visitor(vis));

    Agent a(g);
    std::cout << "Born at: " << a.get_vertex() << std::endl;
    std::cout << "Destination: " << a.get_destination() << std::endl;

    std::vector<Vertex> path = boost::get_dijkstra_shortest_path(a.get_vertex(), a.get_destination(), g);
    int time = 0;
    while(a.get_vertex() != a.get_destination()){
        a.set_vertex(path[path.size()-1-time]);
        std::cout << "Current agent position on the grid: " << a.get_vertex() << " at time  t= " << time << std::endl;
        time++;
    }
}
