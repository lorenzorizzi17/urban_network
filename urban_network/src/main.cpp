#define LOAD_GRAPH true
#define PARSING_MODE false
#define DRAW_CONGESTED false
#define PERCOLATION_THRESHOLD 7
#define ENABLE_GRAPHICS true
#define PROCESS_STATS false
//when building
#define N_NODES 100
#define ROAD_WEIGHT 1
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 10
//dynamical parameter
#define N_AGENTS 3000
#define MAX_SPAWNABLE 15
#define MIN_DIST_DIJKSTRA 5
#define MAX_CAP 15  //CAPIENZA MAX = 5400
#define FLOW_RATE 3 
#define TIME_TO_SLEEP 0 //in ms
#define TIME_MAX_SIMULATION 10000

//user-defined library to run the sim
#include"./headers/ODModel.hpp"
int main()
{
    try {
        od::ODModel od(N_AGENTS);
        od.run_graphics(TIME_MAX_SIMULATION);
    }
    catch (gridlock_exception& e) {
        e.process();
    }
}
