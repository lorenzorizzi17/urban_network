#define N_NODES 100
#define N_AGENTS 100
#define MIN_DIST_DIJKSTRA 40
#define MAX_CAP 8
#define TIME_TO_SLEEP 10
#define FLOW_RATE 3
#define TIME_TO_SIMULATE 40 

#define BOOST_TEST_MODULE MainTest

#include <boost/test/included/unit_test.hpp>
#include "./headers/simulation.hpp"
#include"./headers/simulation.hpp"

BOOST_AUTO_TEST_SUITE(MainTestSuite)

BOOST_AUTO_TEST_CASE(main_execution)
{
    BOOST_CHECK_NO_THROW({
        Simulation s(N_NODES);
        s.run(TIME_TO_SIMULATE);
    });
}

BOOST_AUTO_TEST_SUITE_END()