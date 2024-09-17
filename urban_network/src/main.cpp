//user-defined library to run the sim
#include"./headers/ODModel.hpp"
#include"./headers/gridlock_exception.hpp"

int main()
{
    try {
        ODModel od(N_AGENTS);
        od.run_graphics(TIME_MAX_SIMULATION);
    } catch (gridlock_exception& e) {
        e.process();
    }
}
