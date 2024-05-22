#include<iostream>
#include<algorithm>
#include<random>
#include<map>
#include"./alias.hpp"


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

    Graph make_dual_graph(Graph &g, std::map<Edge, Vertex>& map_dual)
    {
        // let's first compute the total number of edges and create an equal amount of vertices in dual graph
        std::map<Edge, int> map;
        std::map<Vertex,double> map_nodes;
        Graph g_res;
        int c = 0;
        for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++, c++)
        {
            Vertex v = boost::add_vertex(g_res);
            map[*it] = c;
            map_dual[*it] = v;
            map_nodes[v] = get(boost::edge_weight,g)[*it];
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
                    boost::add_edge(j, map[*it], g_res);
                }
            }
            for (Iter_OutEdge it1 = boost::out_edges(target, g).first; it1 != boost::out_edges(target, g).second; it1++)
            {
                int j = map[*it1];
                if ((j != map[*it]) && (!are_connected(map[*it], j, g_res)))
                {
                    boost::add_edge(j, map[*it], g_res);
                }
            }
        }

        //for dijkstra to work, we need to put weights on every edges. Since only nodes here represents something physical (roads), we
        // can just assume that every edge has a weight equal to the sum of weight of every nodes connecting to it
        for (Iter_Edge it = boost::edges(g_res).first; it != boost::edges(g_res).second; it++){
            Vertex v1 = boost::source(*it,g_res);
            Vertex v2 = boost::target(*it,g_res);
            get(boost::edge_weight,g_res)[*it] += double(((map_nodes[v1]+map_nodes[v2]))/2);
        }

        return g_res;
    }

}