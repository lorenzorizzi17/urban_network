#define N_NODES 100
#define N_AGENTS 400
#define MIN_DIST_DIJKSTRA 40
#define MAX_CAP 8
#define TIME_TO_SLEEP 0 //in ms
#define FLOW_RATE 3
#define TIME_MAX_SIMULATION 1000
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 0
#define WAITING_TIME 5
#define LOAD_GRAPH true
#define COLOR_DUAL true

#include"./headers/simulation.hpp"

int main()
{
    Simulation s(N_NODES);
    s.run(TIME_MAX_SIMULATION);
}
