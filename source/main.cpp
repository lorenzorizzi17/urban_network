#define N_NODES 100
#define N_AGENTS 100
#define MIN_DIST_DIJKSTRA 40
#define MAX_CAP 8
#define TIME_TO_SLEEP 0
#define FLOW_RATE 3


#include"./headers/simulation.hpp"

int main()
{
    Simulation s(N_NODES);
    s.printWeights();
    s.run();
}