//user-defined library to run the sim
#include"./headers/ODModel.hpp"
#include"./headers/gridlock_exception.hpp"

int main()
{
    for (int N_AGENTS = 2700; N_AGENTS < 3001; N_AGENTS += 100) {
        int total_gridlocks = 0;
        int free_flows = 0;
        
        std::ofstream general_file;
        general_file.open("data/general/general_T"+ std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS) +".txt",std::ios::app);

        //only when totally congested
        std::ofstream file;
        file.open("data/gridlock_time/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);

        std::ofstream file_gridlock_extension;
        file_gridlock_extension.open("data/congested_cluster_size/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS) + ".txt", std::ios::app);
        
        int const REP = 50;
        for (int i = 0; i < REP; i++) {
            ODModel od();
            std::cout << "N = " << N_AGENTS << ", rep " << i << " ";
            try {
                od.run(TIME_MAX_SIMULATION);
                free_flows++;
                std::cout << "ff";
            }
            catch (gridlock_exception const& e) {
                file << e.getTime() << std::endl;
                file_gridlock_extension << e.getCongestedNodes() << std::endl;
                total_gridlocks++;
                std::cout << "tt";
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