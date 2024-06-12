#define N_NODES 64
#define N_AGENTS 1
#define MIN_DIST_DIJKSTRA 40
#define MAX_CAP 8
#define TIME_TO_SLEEP 100
#define FLOW_RATE 3
#define TIME_MAX_SIMULATION 100
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 5
#define DEBUG_MODE true

#include"./headers/simulation.hpp"

int main()
{
    Simulation s(N_NODES);
    //s.printWeights();
    s.run(TIME_MAX_SIMULATION); 
}