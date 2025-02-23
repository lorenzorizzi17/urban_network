//user-defined library to run the sim
#include"./headers/ODModel.hpp"
#include"./headers/gridlock_exception.hpp"

int main()
{
    try {
        ODModel od;
        od.run_graphics();
    } catch (gridlock_exception& e) {
        e.process();
    }
}