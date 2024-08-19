#define GRAPHICS true
#define PARSE_MODE false
#define N_NODES 100
#define PERCOLATION_THRESHOLD 20
//dynamical parameter
#define N_AGENTS 2500
#define MAX_CAP 15  //CAPIENZA MAX = 5400
#define FLOW_RATE 3 
#define TIME_TO_SLEEP 0 //in ms
#define TIME_MAX_SIMULATION 10000

//teorema di parry/matrice di perry max entropy
#include"./headers/RWModel.hpp"

int main()
{
    rw::RWModel rw(N_AGENTS);
    rw.run(TIME_MAX_SIMULATION);
}
