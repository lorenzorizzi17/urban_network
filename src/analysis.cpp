//user-defined library to run the sim
#include"./headers/ODModel.hpp"
#include"./headers/gridlock_exception.hpp"
#include"omp.h"


int main(int arcg, char* argv[]){

    int n = std::stoi(argv[arcg-1]);
    int TIME = 10000;
    int N_AGENTS = n;

    std::ofstream general_file;
    general_file.open("./data/general/T"+std::to_string(TIME)+"/general_T"+std::to_string(TIME)+"_N" + std::to_string(N_AGENTS) +".txt",std::ios::app);
    std::ofstream file_gridlock_time;
    file_gridlock_time.open("./data/gridlock_time/T"+std::to_string(TIME)+"/gridlocktime_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
    std::ofstream file_gridlock_extension;
    file_gridlock_extension.open("data/congested_cluster_size/T"+std::to_string(TIME)+"/N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
    
    int const REP = 96; // number of repetitions

    omp_set_num_threads(8);
    #pragma omp parallel for
    for(int i = 0; i < REP; i++) {
        // build the model
        ODModel od(N_AGENTS, TIME);
        try {
            // try and run it
            od.run();
            #pragma omp critical
            {
                general_file << " 0 ";
            }
        } catch (gridlock_exception const& e) {
            #pragma omp critical
            {
                file_gridlock_time << e.getTime() << std::endl;
                file_gridlock_extension << e.getCongestedNodes() << std::endl;
                general_file << " 1 ";
            }
        }
    }
}
