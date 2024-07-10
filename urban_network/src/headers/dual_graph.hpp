#ifndef DUAL_GRAPH
#define DUAL_GRAPH

#include<iostream>
#include<algorithm>
#include<random>
#include<map>
#include"./alias.hpp"
#include"update_weights.hpp"


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

//controlla che gli indici siano effettivamente gli stessi. L'ideale sarebbe eliminare la mappa o comunque costruirla in base
//agli indici 

    Graph make_dual_graph(Graph &g, std::map<Vertex, Edge>& map_dual)
    {
        // let's first compute the total number of edges and create an equal amount of vertices in dual graph
        std::map<Edge, int> map;
        std::map<Vertex,double> map_nodes;
        Graph g_res;
        int c = 0;
        for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++, c++)
        {
            Vertex v = boost::add_vertex(g_res);
            get(&VertexProperty::index, g_res)[v] = get(&EdgeProperty::index, g,*it);
            map[*it] = c;
            map_dual[v] = *it;
            map_nodes[v] = get(boost::edge_weight,g)[*it];
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
                    boost::add_edge(map[*it],j, g_res);
                }
            }
        }

        //for dijkstra to work, we need to put weights on every edges. Since only nodes here represents something physical (roads), we
        // can just assume that every edge has a weight equal to the sum of weight of every nodes connecting to it
        for (Iter_Edge it = boost::edges(g_res).first; it != boost::edges(g_res).second; it++){
            Vertex v1 = boost::source(*it,g_res);
            Vertex v2 = boost::target(*it,g_res);
            get(&EdgeProperty::initial_weight,g_res)[*it] += double(((map_nodes[v1]+map_nodes[v2]))/2);
        }
        set_weights(g_res);
        return g_res;
    }

}

#endif