#define PERCOLATION_THRESHOLD 20

#define N_NODES 100
#define N_AGENTS 2000
#define MIN_DIST_DIJKSTRA 5
#define MAX_CAP 15
#define TIME_TO_SLEEP 0 //in ms
#define FLOW_RATE 3
#define TIME_MAX_SIMULATION 10000
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 20
#define WAITING_TIME 5
#define ROAD_WEIGHT 1
#define LOAD_GRAPH true

#include"./headers/RWModel.hpp"

//verify the multi-lane behaviour
/*
void multi_lane_sim() {

    //loading graphs
    graph g(0);
    load_graph("fig/graph.dot", g);
    graph g_dual(0);
    load_dual("fig/graph_dual.dot", g_dual);

    //setting spanw and dest point for 5 agents
    iter_vertex vi = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex dual_vertex) {return g_dual[dual_vertex].index == 0; });
    for (int i = 0; i < 5; i++) {
        rw::add_agents(g_dual, *vi);
    }

    //getting in their way 10 stationary agents
    iter_vertex vstop = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex dual_vertex) {return g_dual[dual_vertex].index == 3; });
    for (int i = 0; i < 7; i++) {
        rw::add_agents(g_dual, *vstop);
    }

    //adding 4 agents sharing the same queue but headed toward a different spot
    iter_vertex vtest = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex dual_vertex) {return g_dual[dual_vertex].index == 24; });
    iter_vertex vtestdest = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex dual_vertex) {return g_dual[dual_vertex].index == 59; });
    for (int i = 0; i < 4; i++) {
        boost::add_agents(g_dual, *vtest, *vtestdest, dual_map);
    }

    //displaying evolution
    std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex v)
        { boost::print_info(v, g, g_dual, 0, dual_map); });
    std::cout << std::endl;
    for (int m_time = 1; m_time < 12; m_time++) {
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex v)
            { boost::flow(v, g_dual, flow_rate, p); });
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex v)
            { boost::print_info(v, g, g_dual, m_time, dual_map); });
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](vertex v)
            { boost::erase_agents(v, g_dual, dual_map); });
        boost::reactivate_flag(g_dual);
        std::cout << std::endl;
    }

}

*/

void prob_counter() {
    //loading graphs
    Graph g(0);
    load_graph("fig/graph.dot", g);
    Graph g_dual(0);
    std::map<Vertex, Edge> dual_map;
    load_dual("fig/parser.dot", g_dual);

    //setting spanw and dest point for 5 agents
    Iter_Vertex vi = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 0; });
    for (int i = 0; i < 20; i++) {
        rw::add_agent(g_dual, *vi);
    }

    std::cout << "Time: " << 0 << std::endl;
    std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
        { rw::print_info(v, g_dual); });
    for (int m_time = 1; m_time < 5; m_time++) {
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
            { rw::flow(v, g_dual, FLOW_RATE); });
        std::cout << "Time: " << m_time << std::endl;
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v){ rw::print_info(v, g_dual); });
        rw::reactivate_flag(g_dual);
        rw::set_flag_vertices(g_dual);
    }


    std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v) {int a = g_dual[v].queue.size(); if (a != 0) { std::cout << double(a) / 5000 << std::endl; } });
}







int main()
{
    prob_counter();
}
