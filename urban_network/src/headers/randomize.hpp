#ifndef RANDOMIZE
#define RANDOMIZE

#include<iostream>
#include<algorithm>
#include<random>
#include"./alias.hpp"
#include"dual_graph.hpp"

namespace boost{

    Iter_Vertex get_random_vertex(Graph const& g){
        Iter_Vertex vi = boost::vertices(g).first;
        int num_e = boost::num_vertices(g);
        std::advance(vi,std::rand()%num_e);
        return vi;
    }

    Iter_Edge get_random_edge(Graph const& g){
        Iter_Edge ei = boost::edges(g).first;
        int num_e = boost::num_edges(g);
        std::advance(ei,std::rand()%num_e);
        return ei;
    }

    
    void add_diagonal_roads(Graph &g, int N, int SIZE)
    {
        // first, select a random vertices in the interior of the direct graph
        for (int k = 0; k < N; k++)
        {
            //selecting a random vertex based on cartesian coordinates (i,j) (only internal nodes)
            int i = 0;int j = 0;
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
                            boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j - 1, g); l = 5;
                            boost::add_edge((i - 1) * SIZE + j - 1, i * SIZE + j, g);
                            break;
                        };
                    case 1:
                        if(!boost::are_connected(i * SIZE + j, (i - 1) * SIZE + j + 1, g)){
                            boost::add_edge(i * SIZE + j, (i - 1) * SIZE + j + 1, g); l = 5;
                            boost::add_edge((i - 1) * SIZE + j + 1, i * SIZE + j, g);
                            break;
                        };
                    case 2:
                        if(!boost::are_connected(i * SIZE + j, (i + 1) * SIZE + j - 1, g)){
                            boost::add_edge(i * SIZE + j, (i + 1) * SIZE + j - 1, g); l = 5;
                            boost::add_edge((i + 1) * SIZE + j - 1, i * SIZE + j, g);
                            break;
                        };
                    case 3:
                        if(!boost::are_connected(i * SIZE + j, (i + 1) * SIZE + j + 1, g)){
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

    void remove_random_edge(Graph &g, int N, int SIZE)
    {
        for (int i = 0; i < N; i++)
        {
            Iter_Edge e = boost::get_random_edge(g);
            Edge e_rev = boost::edge(boost::target(*e, g), boost::source(*e, g), g).first;
            boost::remove_edge(*e, g);
            boost::remove_edge(e_rev, g);
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

    void randomize_weight_map_uniform(Graph &g, double a, double b)
    {
        PropertyMap WeightMap = get(boost::edge_weight, g);
        if (a > b)
        {
            std::cout << "\nBoundaries of the uniform distribution not valid: Function aborted\n";
            return;
        }
        for (int l = 0; l < boost::num_vertices(g); l++)
        {
            for (int m = l+1; m < boost::num_vertices(g); m++)
            
            {
                double rnd = double(double(std::rand()) / RAND_MAX) * (b - a) + a; 
                if (boost::are_connected(l,m,g)){
                    WeightMap[boost::edge(l,m,g).first] = rnd; 
                    if (boost::are_connected(m,l,g)){
                        WeightMap[boost::edge(m,l,g).first] = rnd;
                    }
                }
            }
        }
        
    }
}



#endif