//user-defined library to run the sim
#include"./headers/ODModel.hpp"
#include"./headers/gridlock_exception.hpp"
# include"omp.h"

int main()
{
    for (int N_AGENTS = 2000; N_AGENTS < 2201; N_AGENTS += 100) {

        int total_gridlocks = 0;
        int free_flows = 0;
        
        std::ofstream general_file;
        general_file.open("./data/general/general_T20000_N" + std::to_string(N_AGENTS) +".txt",std::ios::app);

        //only when totally congested
        std::ofstream file;
        file.open("./data/gridlock_time/OD_T20000_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);

        std::ofstream file_gridlock_extension;
        file_gridlock_extension.open("data/congested_cluster_size/OD_T20000_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);

        int const REP = 80;
        omp_set_num_threads(8);

        #pragma omp parallel for shared(free_flows, total_gridlocks)
        for (int i = 0; i < REP; i++) {
            ODModel od(N_AGENTS);
            #pragma omp critical
                std::cout << "Thread n. "<< omp_get_thread_num() << " performing on N = " << N_AGENTS << ", rep. " << i << std::endl;
            try {
                od.run();
                #pragma omp critical
                {
                    free_flows++;
                    std::cout <<  "Thread n. "<< omp_get_thread_num() << " signals ff" << std::endl;
                }
            }
            catch (gridlock_exception const& e) {
                file << e.getTime() << std::endl;
                file_gridlock_extension << e.getCongestedNodes() << std::endl;
                #pragma omp critical
                {
                    total_gridlocks++;
                    std::cout <<  "Thread n. "<< omp_get_thread_num() << " signals tt" << std::endl;
                }
                
            }
            std::cout << std::endl;
        }
       
        //closing file
        file_gridlock_extension.close();
        file.close();

        //gathering data about gridlocks
        general_file << "T: " << total_gridlocks << " FF: " << free_flows << std::endl;
        general_file.close();
    }
}