#define N_NODES 100
#define N_AGENTS 500
#define MIN_DIST_DIJKSTRA 40
#define MAX_CAP 8
#define TIME_TO_SLEEP 100 //in ms
#define FLOW_RATE 3
#define TIME_MAX_SIMULATION 1000
#define N_DIAGONAL_ROADS 10
#define N_REMOVED_ROADS 20
#define WAITING_TIME 5
#define LOAD_GRAPH true

#include"./headers/simulation.hpp"

//verify the multi-lane behaviour
void multi_lane_sim() {

    //loading graphs
    Graph g(0);
    load_graph("fig/graph.dot", g);
    Graph g_dual(0);
    std::map<Vertex, Edge> dual_map;
    load_graph_dual("fig/graph_dual.dot", g_dual, g, dual_map);

    //setting spanw and dest point for 5 agents
    Iter_Vertex vi = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 5; });
    Iter_Vertex vf = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 299; });
    for (int i = 0; i < 5; i++) {
        boost::add_agents(g_dual, *vi, *vf, dual_map);
    }

    //getting in their way 10 stationary agents
    Iter_Vertex vstop = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 3; });
    for (int i = 0; i < 10; i++) {
        boost::add_stationary_agents(g_dual, *vstop, *vf, dual_map, 12);
    }

    //adding 4 agents sharing the same queue but headed toward a different spot
    Iter_Vertex vtest = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 5; });
    Iter_Vertex vtestdest = std::find_if(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex dual_vertex) {return g_dual[dual_vertex].index == 0; });
    for (int i = 0; i < 4; i++) {
        boost::add_agents(g_dual, *vtest, *vtestdest, dual_map);
    }

    //displaying evolution
    std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
        { boost::print_info(v, g, g_dual, 0, dual_map); });
    std::cout << std::endl;
    for (int m_time = 1; m_time < 12; m_time++) {
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
            { boost::flow(v, g_dual, FLOW_RATE); });
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
            { boost::print_info(v, g, g_dual, m_time, dual_map); });
        std::for_each(boost::vertices(g_dual).first, boost::vertices(g_dual).second, [&](Vertex v)
            { boost::erase_agents(v, g_dual, dual_map); });
        boost::reactivate_flag(g_dual);
        std::cout << std::endl;
    }

}




int main()
{
    multi_lane_sim();
}
