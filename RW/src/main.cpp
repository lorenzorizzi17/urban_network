#define GRAPHICS false
#define PARSE_MODE true

#define N_NODES 100
//dynamical parameter
#define MAX_SPAWNABLE 150  //CAPIENZA MAX = 5400
#define MAX_CAP 15
#define FLOW_RATE 3 
#define TIME_TO_SLEEP 0 //in ms
#define TIME_MAX_SIMULATION 10000

//teorema di parry/matrice di perry max entropy
#include"./headers/RWModel.hpp"
#include"./headers/percolation.hpp"

int main()
{
    try {
        int N_AGENTS = 3000;
        int PERCOLATION_THRESHOLD = 800;
        for (int REP = 0; REP < 5; REP++){
            {
                rw::RWModel rw(N_AGENTS, PERCOLATION_THRESHOLD / 100.0);
                rw.run(TIME_MAX_SIMULATION);
                rw.save(1);
            }
            percolation(N_AGENTS, PERCOLATION_THRESHOLD);
        }
    }
    catch (std::runtime_error const& e) {
        std::cout << e.what();
    }
}
