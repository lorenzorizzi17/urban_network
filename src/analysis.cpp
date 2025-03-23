//user-defined library to run the sim
#include"./headers/ODModel.hpp"
#include"./headers/gridlock_exception.hpp"
#include"omp.h"

int main()
{

    int TIME = 10000;
    for (int N_AGENTS = 2900; N_AGENTS < 2901; N_AGENTS += 100) {

        // io stuff
        std::ofstream general_file;
        general_file.open("./data/general/T"+std::to_string(TIME)+"/general_T"+std::to_string(TIME)+"_N" + std::to_string(N_AGENTS) +".txt",std::ios::app);
        std::ofstream file_gridlock_time;
        file_gridlock_time.open("./data/gridlock_time/T"+std::to_string(TIME)+"/gridlocktime_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
        std::ofstream file_gridlock_extension;
        file_gridlock_extension.open("data/congested_cluster_size/T"+std::to_string(TIME)+"/N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
       
        int total_gridlocks = 0;
        int free_flows = 0;

        int const REP = 96;

        omp_set_num_threads(16);
        #pragma omp parallel for shared(free_flows, total_gridlocks)
        for (int i = 0; i < REP; i++) {

            ODModel od(N_AGENTS, TIME);

            #pragma omp critical 
            {
                std::cout << "Thread n. "<< omp_get_thread_num() << " created ODModel" << std::endl;
            }

            try {
                od.run();
                #pragma omp critical
                {
                    free_flows++;
                    std::cout <<  "Thread n. "<< omp_get_thread_num() << " signals ff" << std::endl;
                }
            } catch (gridlock_exception const& e) {
                #pragma omp critical
                {
                    file_gridlock_time << e.getTime() << std::endl;
                    file_gridlock_extension << e.getCongestedNodes() << std::endl;
                    total_gridlocks++;
                    std::cout <<  "Thread n. "<< omp_get_thread_num() << " signals tt" << std::endl;
                }
            }
        }

        
       
        //closing file
        file_gridlock_extension.close();
        file_gridlock_time.close();

        //gathering data about gridlocks
        std::cout << "T: " << total_gridlocks << " FF: " << free_flows << std::endl;
        general_file << "T: " << total_gridlocks << " FF: " << free_flows << std::endl;
        general_file.close();
    }
}