#include <boost/graph/adjacency_list.hpp>
#include<iostream>
#include<algorithm>


typedef boost::adjacency_list<boost::vecS,         //stores out edges of each vertex in a std::list
                              boost::vecS,          //stores vertex set in a std::vector
                              boost::undirectedS,     //graph is undirected
                              boost::no_property,            //vertex property: VertexData
                              boost::property<boost::edge_index_t, double>   //edge property: a weight
                              > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator Iter_Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator Iter_OutEdge;
typedef boost::graph_traits<Graph>::adjacency_iterator Iter_Adj;


bool are_connected(int i, int j, Graph g){
    Iter_Adj iti, itif;
    boost::tie(iti,itif) = boost::adjacent_vertices(i,g);
    for (auto it = iti; it != itif; it++)
    {
        Vertex v2 = *(boost::vertices(g).first+j);
        Vertex v1 = *it;
       if (v1==v2){
            return true;
       };
    }
    return false;
}

Graph make_dual_graph(Graph& g){
    //let's first compute the total number of edges and create an equal amount of vertices in dual graph
    std::map<Edge,int> map;
    Graph g_res;
    int c = 0;
    for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++,c++)
    {   
        Vertex v = boost::add_vertex(g_res);
        map[*it] = c;
    }
std::cout << c;
    
    //now we need to connect those
    for (Iter_Edge it = boost::edges(g).first; it != boost::edges(g).second; it++)
    {
        //elaborate source and target node
        Vertex source = boost::source(*it,g);
        Vertex target = boost::target(*it,g);
        //let's first examine source node
        for (Iter_OutEdge it1 = boost::out_edges(source, g).first; it1 != boost::out_edges(source, g).second; it1++)
        {
            int j = map[*it1];
            if((j!=map[*it])&&(!are_connected(map[*it],j,g_res))){
                //we should check whether those two vertices are already connected
                boost::add_edge(j,map[*it],g_res);
                std::cout << j << " " << map[*it]<< std::endl; 
            }
        }
        for (Iter_OutEdge it1 = boost::out_edges(target, g).first; it1 != boost::out_edges(target, g).second; it1++)
        {
            int j = map[*it1];
            if((j!=map[*it])&&(!are_connected(map[*it],j,g_res))){
                //we should check whether those two vertices are already connected
                boost::add_edge(j,map[*it],g_res);
                std::cout << j << " " << map[*it]<< std::endl; 
            }
        }


        //now let's remove doubles
        /* for (Iter_Edge it = boost::edges(g_res).first; it != boost::edges(g_res).second; it++)
        {
            auto test = std::find_if(boost::edges(g_res).first,boost::edges(g_res).second,[=](Edge e){return *it == e;});
            while (test != boost::edges(g_res).second){
                boost::remove_edge(*test,g_res); //this will invalidate any iterator, e.g it, causing a segfault
                test = std::find_if(boost::edges(g_res).first,boost::edges(g_res).second,[=](Edge e){return *it == e;});
            }
        } */
        

    }
    return g_res;
}