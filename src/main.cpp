//user-defined library to run the sim
#include"./headers/ODModel.hpp"
#include"./headers/gridlock_exception.hpp"
#include"omp.h"

int main()
{
    omp_set_num_threads(8);
    try {
        ODModel od;
        od.run_graphics();
    } catch (gridlock_exception& e) {
        e.process();
    }
}