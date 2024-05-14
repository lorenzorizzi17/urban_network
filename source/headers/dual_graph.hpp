
#include</home/lorenzo17/boost/boost/graph/breadth_first_search.hpp>
#include<iostream>
#include<algorithm>
#include<random>
#include"./vertex.hpp"
#include"./alias.hpp"

// A visitor expanding the built-in function. Overriding function discover_vertex
// The visitor needs to store a property map(graph, property::type)
template <typename PropertyMapVertex>
class bfs_index_printer : public boost::default_bfs_visitor{
private:
    PropertyMapVertex m_name_map;

public:
    bfs_index_printer(PropertyMapVertex n_map) : m_name_map(n_map) {} // ctor

    void discover_vertex(Vertex u, const Graph &g) const
    {
        std::cout << get(m_name_map, u) << " ";
    }
};

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

    template <typename Graph>
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
            while ((i == 0) || (j == 0) || (i == SIZE - 1) || (j == SIZE - 1) || (boost::are_connected(i, j, g)))
            {
                i = std::rand() % (SIZE);
                j = std::rand() % (SIZE);
            }
            int which_diag = std::rand() % 4;
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
    template <typename Graph, typename Edge_it>
    void remove_random_edge(Graph &g, int N, int SIZE)
    {
        for (int i = 0; i < N; i++)
        {
            Edge_it iti = boost::edges(g).first;
            int rnd = std::rand() % (boost::num_edges(g));
            std::advance(iti, rnd);
            boost::remove_edge(*iti, g);
        }
    }

    template <typename Graph, typename Edge_it>
    void print_weight_map(Graph &g)
    {
        PropertyMap WeightMap = get(boost::edge_weight, g);
        for (Edge_it it_e = boost::edges(g).first; it_e != boost::edges(g).second; it_e++)
        {
            std::cout << "Edge that connects node " << boost::source(*it_e, g) << " to " << boost::target(*it_e, g) << " has a weight = " << WeightMap[*it_e] << std::endl;
        }
    }

    template <typename Graph, typename Edge_it>
    void randomize_weight_map_uniform(Graph &g, double a, double b)
    {
        PropertyMap WeightMap = get(boost::edge_weight, g);
        if (a > b)
        {
            std::cout << "\nBoundaries of the uniform distribution not valid: Function aborted\n";
            return;
        }
        for (Edge_it it_e = boost::edges(g).first; it_e != boost::edges(g).second; it_e++)
        {
            double rnd = double(double(std::rand()) / RAND_MAX) * (b - a) + a;
            WeightMap[*it_e] = rnd;
        }
    }

}