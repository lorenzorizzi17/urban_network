// this file will implement the agent perspective in a OD model

#ifndef AGENTS
#define AGENTS

#include "./alias.hpp"
#include "./randomize.hpp"
#include "./dijkstra.hpp"

/* void add_agent(Iter_Edge const& e, Graph& g, Agent const& a){
  AgentMap pmap = boost::get(boost::edge_agents,g);
  pmap[*e].push_back(a);
} */



class Agent
{
private:
    Vertex m_curr_road; // vertex in dual graph = road
    Vertex m_dest_road;
    std::vector<Vertex> m_path;
    int m_internal_time = 1;
    int m_id = 0;
    std::map<Vertex, Edge> m_dual_map;

public:
    static int m_instances;
    bool m_trav = false;
    // std::map here is just for debug
    Agent(Graph &g_dual, Vertex spawn_point, double MIN_DIST, std::map<Vertex, Edge> &dual_map)
    {
        m_internal_time = 1;
        m_id = m_instances;
        m_instances++;
        m_dual_map = dual_map;
        // randomly chose first vertex
        m_curr_road = spawn_point;
        // randomly chose destination vertex and compute dijkstra shortest path
        double distance = 0;
        std::tie(m_dest_road, m_path, distance) = boost::get_vertex_based_on_dijkstra_shortest_path(m_curr_road, g_dual, MIN_DIST);
        // prints some info
        std::cout << "Agents n. " << this->get_id() << " born in road: " << this->get_road() << ", dest in " << this->get_road_destination() << " (d = " << distance << ")" << std::endl;
    }

    Vertex get_vertex() const
    {
        return m_curr_road;
    }

    Edge get_road() const
    {
        return m_dual_map.at(m_curr_road);
    }

    Edge get_road_destination() const
    {
        return m_dual_map.at(m_dest_road);
    }

    void set_edge(Vertex const &v)
    {
        m_curr_road = v;
    }

    std::vector<Vertex> get_route() const
    {
        return m_path;
    }

    void evolve_dijsktra()
    {
        m_curr_road = m_path[m_internal_time];
        m_internal_time++;
        m_trav = true;
    }

    bool arrived() const
    {
        return m_curr_road == m_dest_road;
    }

    int get_id() const
    {
        return m_id;
    }

    bool has_traveled() const
    {
        return m_trav;
    }

    void set_traveled(bool b)
    {
        m_trav = b;
    }
};


namespace boost
{
    void add_agents(Graph &g_dual, int N_AGENTS, double MIN_DIST, std::map<Vertex, Edge> dual_map)
    {
        for (int i = 0; i < N_AGENTS; i++)
        {
            Vertex v = *boost::get_random_vertex(g_dual);
            boost::get(boost::vertex_agents, g_dual, v).push_back(std::make_shared<Agent>(g_dual,v,MIN_DIST,dual_map));
        }
    }

    /* void remove_arrived_agents(Graph &g_dual)
    {
        for (auto it = boost::vertices(g_dual).first; it != boost::vertices(g_dual).second; it++)
        {
            boost::get(boost::vertex_agents, g_dual, *it).erase(std::remove_if(boost::get(boost::vertex_agents, g_dual, *it).begin(), boost::get(boost::vertex_agents, g_dual, *it).end(), [](Agent const& a)
                                                                               { return a.arrived(); }),
                                                                boost::get(boost::vertex_agents, g_dual, *it).end());
        }
    } */
}

#endif