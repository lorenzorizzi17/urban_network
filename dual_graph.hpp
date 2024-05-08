#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <algorithm>
#include <random>

typedef boost::adjacency_list<boost::vecS,                                 // stores out edges of each vertex in a std::list
                              boost::vecS,                                 // stores vertex set in a std::vector
                              boost::undirectedS,                          // graph is undirected
                              boost::no_property,                          // vertex property: VertexData
                              boost::property<boost::edge_index_t, double> // edge property: a weight
                              >
    Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator Iter_Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator Iter_OutEdge;
typedef boost::graph_traits<Graph>::adjacency_iterator Iter_Adj;

namespace boost
{
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

    Graph make_dual_graph(Graph &g)
    {
        // let's first compute the total number of edges and create an equal amount of vertices in dual graph
        std::map<Edge, int> map;
        Graph g_res;
        int c = 0;
        for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++, c++)
        {
            Vertex v = boost::add_vertex(g_res);
            map[*it] = c;
        }

        // now we need to connect those
        for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++)
        {
            // elaborate source and target node
            Vertex source = boost::source(*it, g);
            Vertex target = boost::target(*it, g);
            // let's first examine source node
            for (Iter_OutEdge it1 = boost::out_edges(source, g).first; it1 != boost::out_edges(source, g).second; it1++)
            {
                int j = map[*it1];
                if ((j != map[*it]) && (!are_connected(map[*it], j, g_res)))
                {
                    // we should check whether those two vertices are already connected
                    boost::add_edge(j, map[*it], g_res);
                }
            }
            for (Iter_OutEdge it1 = boost::out_edges(target, g).first; it1 != boost::out_edges(target, g).second; it1++)
            {
                int j = map[*it1];
                if ((j != map[*it]) && (!are_connected(map[*it], j, g_res)))
                {
                    // we should check whether those two vertices are already connected
                    boost::add_edge(j, map[*it], g_res);
                }
            }
        }
        return g_res;
    }

    template <typename Graph, typename Iter_E>
    void add_diagonal_roads(Graph &g, int N, int SIZE)
    {
        // first, select a random vertices in the interior of the graph
        for (int k = 0; k < N; k++)
        {
            int i, j = 0;
            while ((i == 0) || (j == 0) || (i == SIZE - 1) || (j == SIZE - 1))
            {
                i = std::rand() % (SIZE);
                j = std::rand() % (SIZE);
            }
            std::cerr << "chosen nodes: " << i << " " << j << std::endl;
            int which_diag = std::rand() % 4;
            std::cerr << "which diag? " << which_diag;
            switch (which_diag)
            {
            case 0:
                boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j - 1, g);
                break;
            case 1:
                boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j + 1, g);
                break;
            case 2:
                boost::add_edge(i * SIZE + j, (i + 1) * SIZE + j - 1, g);
                break;
            case 3:
                boost::add_edge(i * SIZE + j, (i + 1) * SIZE + j + 1, g);
                break;
            default:
                break;
            }
        }
    }

    /* int num_edges(Graph g){
        Iter_Edge iti, itf;
        boost::tie(iti,itf) = boost::edges(g);
        return std::distance(itf,iti);
    }
 */
    template<typename Graph,typename Edge_it>
    void remove_random_edge(Graph& g, int N, int SIZE){
        for (int i = 0; i < N; i++)
        {
            Iter_Edge iti = boost::edges(g).first;
            int rnd = std::rand() % (boost::num_edges(g));
std::cerr << " n.edge: " << num_edges(g) << "j";
            std::advance(iti, rnd);
            boost::remove_edge(*iti,g);
        }
    }
}