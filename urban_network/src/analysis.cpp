#define LOAD_GRAPH true
#define PARSING_MODE false
#define DRAW_CONGESTED false
#define ENABLE_GRAPHICS false
#define PERCOLATION_THRESHOLD 7
#define PROCESS_STATS true
//when building
#define N_NODES 100
#define ROAD_WEIGHT 1
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 10
//dynamical parameter
#define MAX_SPAWNABLE 150
#define MIN_DIST_DIJKSTRA 5
#define MAX_CAP 15  //CAPIENZA MAX = 5400
#define FLOW_RATE 3 
#define TIME_TO_SLEEP 0 //in ms
#define TIME_MAX_SIMULATION 20000

//user-defined library to run the sim
#include"./headers/ODModel.hpp"


int main()
{
    for (int N_AGENTS = 2300; N_AGENTS < 2401; N_AGENTS += 100) {
        int total_gridlocks = 0;
        int partial_gridlocks = 0;
        int free_flows = 0;
        
        std::ofstream general_file;
        general_file.open("fig/general/general_T"+ std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS) +".txt",std::ios::app);

        //only when totally congested
        std::ofstream file;
        file.open("fig/gridlock_time/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);

        std::ofstream file_gridlock_extension;
        file_gridlock_extension.open("fig/congested_cluster_size/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
        std::ofstream file_partial_gridlock;
        file_partial_gridlock.open("fig/congested_cluster_size/partialOD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);

        int const REP = 100;
        for (int i = 0; i < REP; i++) {
            od::ODModel od(N_AGENTS);
            std::cout << i << " ";
            try {
                od.run(TIME_MAX_SIMULATION);
                od.save(i);
                free_flows++;
                std::cout << "ff";
            }
            catch (gridlock_exception const& e) {
                //od.print_total_gridlock();
                file << e.getTime() << std::endl;
                file_gridlock_extension << e.getCongestedNodes() << std::endl;
                total_gridlocks++;
                std::cout << "tt";
            }
            catch (partial_gridlock_exception const& ep) {
                //od.print_partial_gridlock();
                file_partial_gridlock << ep.getCongestedNodes() << std::endl;
                partial_gridlocks++;
                std::cout << "pp";
            }
            std::cout << std::endl;
        }
       
        //closing file
        file_gridlock_extension.close();
        file_partial_gridlock.close();
        file.close();
        //gathering data about gridlocks
        general_file << "T: " << total_gridlocks << " P: " << partial_gridlocks << " FF: " << free_flows << std::endl;
        general_file.close();
    }
}
