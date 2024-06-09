#define N_NODES 100
#define N_AGENTS 150
#define MIN_DIST_DIJKSTRA 40
#define MAX_CAP 10

#include"./headers/simulation.hpp"


int main()
{
    Simulation s(N_NODES);
    s.run();
}