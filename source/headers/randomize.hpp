#ifndef RANDOMIZE
#define RANDOMIZE

#include<iostream>
#include<algorithm>
#include<random>
#include"./alias.hpp"
#include"dual_graph.hpp"

namespace boost{

    Edge get_random_edge(Graph const& g){
        Iter_Edge ei = boost::edges(g).first;
        int num_e = std::distance(boost::edges(g).first, boost::edges(g).second);
        std::advance(ei,std::rand()/num_e);
        return *ei;
    }

    Iter_Vertex get_random_vertex(Graph const& g){
        Iter_Vertex vi = boost::vertices(g).first;
        int num_e = boost::num_vertices(g);
        std::advance(vi,std::rand()%num_e);
        return vi;
    }
    

    template <typename Graph, typename Iter_E>
    void add_diagonal_roads(Graph &g, int N, int SIZE)
    {
        // first, select a random vertices in the interior of the graph
        for (int k = 0; k < N; k++)
        {
            //selecting a random vertex based on cartesian coordinates (i,j)
            int i, j = 0;
            while (((i == 0) || (j == 0) || (i == SIZE - 1) || (j == SIZE - 1)))
            {
                i = std::rand() % (SIZE);
                j = std::rand() % (SIZE);
            }
            int which_diag = std::rand() % 4;
            for(int l = 0;l<4;l++){
                switch (which_diag){
                    case 0:
                        if(!boost::are_connected(i * SIZE + j, (i - 1) * SIZE + j - 1, g)){
                            boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j - 1, g); l =5;
                            break;
                        };
                    case 1:
                        if(!boost::are_connected(i * SIZE + j, (i - 1) * SIZE + j + 1, g)){
                            boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j + 1, g); l = 5;
                            break;
                        };
                    case 2:
                        if(!boost::are_connected(i * SIZE + j, (i + 1) * SIZE + j - 1, g)){
                            boost::add_edge(i * SIZE + j, (i + 1) * SIZE + j - 1, g); l = 5;
                            break;
                        };
                    case 3:
                        if(!boost::are_connected(i * SIZE + j, (i + 1) * SIZE + j + 1, g)){
                            boost::add_edge(i * SIZE + j, (i + 1) * SIZE + j + 1, g); l = 5;
                            break;
                        };
                    default:
                        break;
                    }
                which_diag = std::rand() % 4;
            }
        }
    }

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

    void print_weight_map(Graph &g)
    {
        PropertyMap WeightMap = get(boost::edge_weight, g);
        for (Iter_Edge it_e = boost::edges(g).first; it_e != boost::edges(g).second; it_e++)
        {
            std::cerr << "Edge that connects node " << boost::source(*it_e, g) << " to " << boost::target(*it_e, g) << " has a weight = " << WeightMap[*it_e] << std::endl;
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



#endif