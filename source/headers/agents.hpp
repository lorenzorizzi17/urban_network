//this file will implement the agent perspective in a OD model

#ifndef AGENTS
#define AGENTS


#include"./alias.hpp"
#include"./randomize.hpp"
#include"./dijkstra.hpp"

void add_agent(Iter_Edge const& e, Graph& g, Agent const& a){
  AgentMap pmap = boost::get(boost::edge_agents,g);
  pmap[*e].push_back(a);
}


class Agent {
    private:
    Vertex m_curr_road;
    Vertex m_dest_road;
    std::vector<Vertex> m_path;
    int m_internal_time = 1;
    int m_id = 0;
    std::map<Vertex,Edge> m_dual_map;
    

    public:
    static int m_instances;
//std::map here is just for debug
    Agent(Graph& g_dual, double MIN_DIST, std::map<Vertex,Edge>& dual_map) {
        m_internal_time = 1;
        m_id = m_instances;
        m_instances++;
        m_dual_map = dual_map;
        //randomly chose first vertex
        m_curr_road = *boost::get_random_vertex(g_dual);
        //randomly chose destination vertex and compute dijkstra shortest path
        double distance = 0;
        std::tie(m_dest_road,m_path,distance) = boost::get_vertex_based_on_dijkstra_shortest_path(m_curr_road,g_dual,MIN_DIST);
        //prints some info
        std::cout << "Agents n. " << this->get_id() << " born in road: " << this->get_road() << ", dest in " << this->get_road_destination() << " (d = " << distance << ")" << std::endl; 

    }
    
    Edge get_road() const {
        return m_dual_map.at(m_curr_road);
    }

    Edge get_road_destination() const {
        return m_dual_map.at(m_dest_road);
    }


    void set_edge(Vertex const& v){
        m_curr_road = v;
    }

    std::vector<Vertex> get_route() const{
        return m_path;
    }

    void evolve_dijsktra(){
        m_curr_road = m_path[m_internal_time];
        m_internal_time++;
    }

    bool arrived() const{
        return m_curr_road == m_dest_road;
    }

    int get_id()const{  
        return m_id;
    }

    /* double get_distance_left(Graph const& g,Graph const& g_dual, std::map<Edge,Vertex>& map){
        return boost::get_dijkstra_shortest_path(m_curr_edge,m_dest_edge,g,g_dual, map).second;
    } */

};

#endif