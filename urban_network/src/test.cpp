#define N_NODES 100
#define N_AGENTS 1
#define MIN_DIST_DIJKSTRA 40
#define MAX_CAP 8
#define TIME_TO_SLEEP 0
#define FLOW_RATE 3
#define TIME_MAX_SIMULATION 300
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 20
#define WAITING_TIME 5
#define DEBUG_MODE true
#define LOAD_GRAPH true

#define BOOST_TEST_MODULE MyCodeTest
#define NOMINMAX

#include <boost/test/included/unit_test.hpp>
#include "./headers/simulation.hpp"

BOOST_AUTO_TEST_SUITE(MyCodeTestSuite)

BOOST_AUTO_TEST_CASE(loading_graphs) {
    Graph g(0);
    load_graph("fig/graph.dot", g);
    Graph g_dual(0);
    std::map<Vertex, Edge> dual_map;
    load_graph_dual("fig/graph_dual.dot", g_dual, g, dual_map);
    for (const std::pair<Vertex, Edge>& pair : dual_map) {
        BOOST_CHECK_EQUAL(g_dual[pair.first].index, g[pair.second].index);
        //std::cout << "Vertex/Road : " << g_dual[pair.first].index << " connects crosspoints: " << g[boost::source(pair.second,g)].index << " to " << g[boost::target(pair.second, g)].index <<  std::endl;
    }

    //std::cout << "Printing graph ... \n";
    print_graph(g, g_dual, dual_map);

    Graph new_g = Graph(0);
    Graph new_g_dual = Graph(0);
    std::map<Vertex, Edge> new_dual_map;
    load_graph("fig/graph.dot", new_g);
    load_graph_dual("fig/graph_dual.dot", new_g_dual, new_g, new_dual_map);
    for (const std::pair<Vertex, Edge>& pair : new_dual_map) {
        BOOST_CHECK_EQUAL(new_g_dual[pair.first].index, new_g[pair.second].index);
        //std::cout << "Vertex/Road : " << new_g_dual[pair.first].index << " connects crosspoints: " << new_g[boost::source(pair.second, new_g)].index << " to " << new_g[boost::target(pair.second, new_g)].index << std::endl;
        auto it = std::find_if(dual_map.begin(), dual_map.end(), [&](std::pair<Vertex, Edge> const& pairr) {return g_dual[pairr.first].index == new_g_dual[pair.first].index; });
        BOOST_CHECK_EQUAL(g[boost::source(it->second,g)].index, new_g[boost::source(pair.second,new_g)].index);
        BOOST_CHECK_EQUAL(g[boost::target(it->second, g)].index, new_g[boost::target(pair.second, new_g)].index);
    }
}

BOOST_AUTO_TEST_CASE(sandbox) {
    Graph g(0);
    load_graph("fig/graph.dot", g);
    Graph g_dual(0);
    std::map<Vertex, Edge> dual_map;
    load_graph_dual("fig/graph_dual.dot", g_dual, g, dual_map);
    Iter_Vertex vi = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 5; });
    Iter_Vertex vf = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 299; });
    for (int i = 0; i < 5; i++) {
        boost::add_agents(g_dual, *vi, *vf, dual_map);
    }

    Iter_Vertex vstop = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 3; });
    for (int i = 0; i < 10; i++) {
        boost::add_agents_stationary(g_dual, *vstop, *vf, dual_map,12);
    }

    Iter_Vertex vtest = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 5; });
    Iter_Vertex vtestdest = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 0; });
    boost::add_agents(g_dual, *vtest, *vtestdest, dual_map);


    std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
        { boost::print_info(v, g, g_dual, 0, dual_map); });
    for (int m_time = 1; m_time < 12; m_time++) {
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
            { boost::flow(v, g_dual, FLOW_RATE); });
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
            { boost::print_info(v, g, g_dual, m_time, dual_map); });
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
            { boost::erase_agents(v, g_dual, dual_map); });
        boost::reactivate_flag(g_dual);
    }
}


BOOST_AUTO_TEST_SUITE_END()

