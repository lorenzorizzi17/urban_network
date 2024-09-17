#include<string>
#include<map>
#include<vector>
#include<algorithm>
#include<fstream>
#include"headers/graph_builder.hpp"
#include"headers/graph_utilities.hpp"

#include <boost/property_map/dynamic_property_map.hpp>
#include <libs/graph/src/read_graphviz_new.cpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

std::string check(std::pair<int, int> const& p, std::vector<std::pair<int, int>>& v, bool eof) {
    auto it = std::find_if(v.begin(), v.end(), [&](std::pair<int, int> const& pa) {return ((p.first == pa.first) && (p.second == pa.second)) || ((p.first == pa.second) && (p.second == pa.first)); });
    if (true) {
        v.push_back(p);
        if (eof) {
            return (std::to_string(p.first) + " " + std::to_string(p.second));
        }
        else {
            return ('\n' + std::to_string(p.first) + " " + std::to_string(p.second));
        }
    }
    else {
        return "";
    }
}

bool are_connected(int i, int j, Graph g)
{
    Iter_Adj iti, itif;
    boost::tie(iti, itif) = boost::adjacent_vertices(i, g);
    for (auto it = iti; it != itif; it++)
    {
        Vertex v2 = *(boost::vertices(g).first + j);
        Vertex v1 = *it;
        if (v1 == v2)
        {
            return true;
        };
    }
    return false;
}

void create_manhattan_data(const int SIZE) {
    std::ofstream file_in("data/manhattan_grid.dat", std::ios::trunc);
    std::vector<std::pair<int, int>> v;
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        int row = i / SIZE;
        int column = i - row * (SIZE);
        if (column == 0) {
            if (row == 0) {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column + 1);
                std::pair<int, int> p2(row * SIZE + column, (row + 1) * SIZE + column);
                file_in << check(p1, v, true) << check(p2, v, false);
            }
            else if (row == SIZE - 1) {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column + 1);
                std::pair<int, int> p2(row * SIZE + column, (row - 1) * SIZE + column);
                file_in << check(p1, v, false) << check(p2, v, false);
            }
            else {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column + 1);
                std::pair<int, int> p2(row * SIZE + column, (row - 1) * SIZE + column);
                std::pair<int, int> p3(row * SIZE + column, (row + 1) * SIZE + column);
                file_in << check(p1, v, false) << check(p2, v, false) << check(p3, v, false);
            }
        }
        else if (column == SIZE - 1) {
            if (row == 0) {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column - 1);
                std::pair<int, int> p2(row * SIZE + column, (row + 1) * SIZE + column);
                file_in << check(p1, v, false) << check(p2, v, false);
            }
            else if (row == SIZE - 1) {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column - 1);
                std::pair<int, int> p2(row * SIZE + column, (row - 1) * SIZE + column);
                file_in << check(p1, v, false) << check(p2, v, false);
            }
            else {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column - 1);
                std::pair<int, int> p2(row * SIZE + column, (row - 1) * SIZE + column);
                std::pair<int, int> p3(row * SIZE + column, (row + 1) * SIZE + column);
                file_in << check(p1, v, false) << check(p2, v, false) << check(p3, v, false);
            }
        }
        else {
            if (row == 0) {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column - 1);
                std::pair<int, int> p2(row * SIZE + column, (row + 1) * SIZE + column);
                std::pair<int, int> p3(row * SIZE + column, row * SIZE + column + 1);
                file_in << check(p1, v, false) << check(p2, v, false) << check(p3, v, false);
            }
            else if (row == SIZE - 1) {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column - 1);
                std::pair<int, int> p2(row * SIZE + column, (row - 1) * SIZE + column);
                std::pair<int, int> p3(row * SIZE + column, row * SIZE + column + 1);
                file_in << check(p1, v, false) << check(p2, v, false) << check(p3, v, false);
            }
            else {
                std::pair<int, int> p1(row * SIZE + column, row * SIZE + column - 1);
                std::pair<int, int> p2(row * SIZE + column, (row + 1) * SIZE + column);
                std::pair<int, int> p3(row * SIZE + column, row * SIZE + column + 1);
                std::pair<int, int> p4(row * SIZE + column, (row - 1) * SIZE + column);
                file_in << check(p1, v, false) << check(p2, v, false) << check(p3, v, false) << check(p4, v, false);
            }
        }
    }
}

void add_diagonal_roads(Graph& g, int N, int SIZE)
{
    // first, select a random vertices in the interior of the direct graph
    for (int k = 0; k < N; k++)
    {
        //selecting a random vertex based on cartesian coordinates (i,j) (only internal nodes)
        int i = 0; int j = 0;
        while (((i == 0) || (j == 0) || (i == SIZE - 1) || (j == SIZE - 1)))
        {
            i = std::rand() % (SIZE);
            j = std::rand() % (SIZE);
        }
        int which_diag = std::rand() % 4;
        for (int l = 0; l < 4; l++) {
            switch (which_diag) {
            case 0:
                if (!are_connected(i * SIZE + j, (i - 1) * SIZE + j - 1, g)) {
                    boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j - 1, g); l = 5;
                    boost::add_edge((i - 1) * SIZE + j - 1, i * SIZE + j, g);
                    break;
                };
            case 1:
                if (!are_connected(i * SIZE + j, (i - 1) * SIZE + j + 1, g)) {
                    boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j + 1, g); l = 5;
                    boost::add_edge((i - 1) * SIZE + j + 1, i * SIZE + j, g);
                    break;
                };
            case 2:
                if (!are_connected(i * SIZE + j, (i + 1) * SIZE + j - 1, g)) {
                    boost::add_edge(i * SIZE + j, (i + 1) * SIZE + j - 1, g); l = 5;
                    boost::add_edge((i + 1) * SIZE + j - 1, i * SIZE + j, g);
                    break;
                };
            case 3:
                if (!are_connected(i * SIZE + j, (i + 1) * SIZE + j + 1, g)) {
                    boost::add_edge(i * SIZE + j, (i + 1) * SIZE + j + 1, g); l = 5;
                    boost::add_edge((i + 1) * SIZE + j + 1, i * SIZE + j, g);
                    break;
                };
            default:
                break;
            }
            which_diag = std::rand() % 4;
        }
    }
}

void remove_random_edge(Graph& g, int N, int SIZE)
{
    for (int i = 0; i < N; i++)
    {
        Iter_Edge e = get_random_edge(g);
        Edge e_rev = boost::edge(boost::target(*e, g), boost::source(*e, g), g).first;
        boost::remove_edge(*e, g);
        boost::remove_edge(e_rev, g);
    }
}


Graph make_dual_graph(Graph& g, std::map<Vertex, Edge>& map_dual)
{
    // let's first compute the total number of edges and create an equal amount of vertices in dual graph
    std::map<Edge, int> map;
    std::map<Vertex, double> map_nodes;
    Graph g_res;
    int c = 0;
    for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++, c++)
    {
        Vertex v = boost::add_vertex(g_res);
        get(&VertexProperty::index, g_res)[v] = get(&EdgeProperty::index, g, *it);
        map[*it] = c;
        map_dual[v] = *it;
        map_nodes[v] = get(boost::edge_weight, g)[*it];
    }

    // now we need to connect those
    for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++)
    {
        Vertex target = boost::target(*it, g);
        for (Iter_OutEdge it1 = boost::out_edges(target, g).first; it1 != boost::out_edges(target, g).second; it1++)
        {
            int j = map[*it1];
            if ((j != map[*it]) && (!are_connected(map[*it], j, g_res)))
            {
                boost::add_edge(map[*it], j, g_res);
            }
        }
    }

    //for dijkstra to work, we need to put weights on every edges. Since only nodes here represents something physical (roads), we
    // can just assume that every edge has a weight equal to the sum of weight of every nodes connecting to it
    for (Iter_Edge it = boost::edges(g_res).first; it != boost::edges(g_res).second; it++) {
        Vertex v1 = boost::source(*it, g_res);
        Vertex v2 = boost::target(*it, g_res);
        get(&EdgeProperty::initial_weight, g_res)[*it] += double(((map_nodes[v1] + map_nodes[v2])) / 2);
    }
    for (auto it = boost::edges(g_res).first; it != boost::edges(g_res).second; it++) {
        get(boost::edge_weight, g_res, *it) = get(&EdgeProperty::initial_weight, g_res, *it);
    }
    return g_res;
}




void randomize_weight_map_uniform(Graph& g, double a, double b)
{
    auto WeightMap = get(&EdgeProperty::initial_weight, g);
    if (a > b)
    {
        std::cout << "\nBoundaries of the uniform distribution not valid: Function aborted\n";
        return;
    }
    for (int l = 0; l < boost::num_vertices(g); l++)
    {
        for (int m = l + 1; m < boost::num_vertices(g); m++)

        {
            double rnd = double(double(std::rand()) / RAND_MAX) * (b - a) + a;
            if (are_connected(l, m, g)) {
                WeightMap[boost::edge(l, m, g).first] = rnd;
                if (are_connected(m, l, g)) {
                    WeightMap[boost::edge(m, l, g).first] = rnd;
                }
            }
        }
    }

}

void set_weights(Graph& dual) {
    //iterate on edges
    for (auto it = boost::edges(dual).first; it != boost::edges(dual).second; it++) {
        get(boost::edge_weight, dual, *it) = get(&EdgeProperty::initial_weight, dual, *it);
    }
}

Graph build_graph(Config const& config) {
    // creating manhattan data
	int N = config.N_NODES;
    create_manhattan_data(std::sqrt(N));

    // storing the graph based on previous operation on .txt file
    Graph g(N);
    for (int i = 0; i < N; i++) {
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
    add_diagonal_roads(g, config.N_DIAGONAL_ROADS, std::sqrt(N));
    remove_random_edge(g, config.N_REMOVED_ROADS, std::sqrt(N));
    int c = 0;
    for (auto it = boost::edges(g).first; it != boost::edges(g).second; it++, c++) {
        get(&EdgeProperty::index, g, *it) = c;
    }
    // randomize edges weight
    randomize_weight_map_uniform(g, config.ROAD_WEIGHT - 0.1, config.ROAD_WEIGHT + 0.1); // as for now, uniform distribution. Maybe gaussian?
    set_weights(g);
    // print_weight_map<Graph, Iter_Edge>(g);
    return g;
}


void load_graph_dual(const std::string& filename, Graph& dual, Graph& graph, std::map<Vertex, Edge>& dual_map) {
    std::ifstream dot_file(filename);

    boost::dynamic_properties dp(boost::ignore_other_properties);
    auto index_map = get(&VertexProperty::index, dual);
    auto weight_map = get(&EdgeProperty::initial_weight, dual);

    dp.property("label", index_map);
    dp.property("weight", weight_map);

    if (!boost::read_graphviz(dot_file, dual, dp)) {
        std::cerr << "Error reading graphviz file: " << filename << std::endl;
    }

    for (auto it = boost::vertices(dual).first; it != boost::vertices(dual).second; it++) {
        int index = get(&VertexProperty::index, dual, *it);
        auto edg = std::find_if(boost::edges(graph).first, boost::edges(graph).second, [&](Edge e) {
            return (get(&EdgeProperty::index, graph, e) == index); });
        dual_map[*it] = *edg;
    }

    set_weights(dual);
}


void load_graph(const std::string& filename, Graph& graph) {
    std::ifstream dot_file(filename);

    boost::dynamic_properties dp(boost::ignore_other_properties);

    auto index_map_vert = get(&VertexProperty::index, graph);
    auto index_map_edge = get(&EdgeProperty::index, graph);
    auto weight_map = get(&EdgeProperty::initial_weight, graph);
    dp.property("label", index_map_vert);
    dp.property("label_edge", index_map_edge);
    dp.property("weight", weight_map);

    if (!boost::read_graphviz(dot_file, graph, dp)) {
        std::cerr << "Error reading graphviz file: " << filename << std::endl;
    }

    set_weights(graph);
}


void print_graph(Graph const& g, Graph const& dual, std::map<Vertex, Edge> const& dual_map) {
    std::ofstream file("fig/graph.dot");
    auto edge_writer = [&](std::ostream& out, const Edge e) {
        out << "[weight=\"" << boost::get(&EdgeProperty::initial_weight, g, e) << "\", label_edge= \"" << boost::get(&EdgeProperty::index, g, e) << "\"]";
        };

    auto edge_writer_dual = [&](std::ostream& out, const Edge e) {
        out << "[weight=\"" << boost::get(&EdgeProperty::initial_weight, dual, e) << "\"]";
        };

    auto vertex_writer = [&](std::ostream& out, const auto& v) {
        out << "[label=\"" << boost::get(&VertexProperty::index, g, v) << "\"]";
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