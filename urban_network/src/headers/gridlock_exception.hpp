#ifndef GRIDLOCK_EXCEPTION
#define GRIDLOCK_EXCEPTION

#include <exception>
#include "alias.hpp"

//exception for gridlock collapse
class gridlock_exception : public std::exception {
    private:
        int time;
        int congested_nodes;
    public:
        gridlock_exception(int, int);
        int getTime() const;
        int getCongestedNodes() const;
        void process();
};

//free-function that check for gridlock
void check_for_gridlock(Graph&, int);

#endif