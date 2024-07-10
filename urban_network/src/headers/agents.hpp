#ifndef AGENTS
#define AGENTS

#include "./alias.hpp"
#include "./randomize.hpp"
#include "./dijkstra.hpp"

/* void add_agent(Iter_Edge const& e, Graph& g, Agent const& a){
  AgentMap pmap = boost::get(boost::edge_agents,g);
  pmap[*e].push_back(a);
} */

namespace boost
{
    void add_agents(Graph &g_dual, int N, std::map<Vertex, Edge> &dual_map)
    {
        for (int i = 0; i < N; i++)
        {
            Vertex v = *boost::get_random_vertex(g_dual);
            boost::get(&VertexProperty::queue, g_dual, v).push_back(std::make_shared<Agent>(g_dual, v, dual_map));
        }
    }

    
}

class Agent
{
private:
    Vertex m_curr_road; // vertex in dual graph = road
    Vertex m_dest_road;
    std::vector<Vertex> m_path;
    int m_internal_time = 1;
    int m_id = 0;
    std::map<Vertex, Edge> m_dual_map;
    Graph *m_dual;

public:
    static int m_instances;
    bool m_trav = false;
    // std::map here is just for debug
    Agent(Graph &g_dual, Vertex spawn_point, std::map<Vertex, Edge> dual_map)
    {
        m_internal_time = 1;
        m_id = m_instances;
        m_instances++;
        m_dual_map = dual_map;
        m_dual = &g_dual;
        // randomly chose first vertex
        m_curr_road = spawn_point;
        // randomly chose destination vertex and compute dijkstra shortest path
        double distance = 0;
        std::tie(m_dest_road, m_path, distance) = boost::get_vertex_based_on_dijkstra_shortest_path(m_curr_road, g_dual);
        // prints some info
        //DEBUG("Agents n. " + std::to_string(this->get_id()) + " born in road: " + std::to_string(g_dual[this->get_vertex()].index) + ", dest in " + std::to_string(g_dual[this->get_vertex_destination()].index) + " (d = " + std::to_string(distance) + ")");
    }

    Agent(Graph& g_dual, Vertex spawn_point, Vertex dest_point, std::map<Vertex, Edge> dual_map)
    {
        m_internal_time = 1;
        m_id = m_instances;
        m_instances++;
        m_dual_map = dual_map;
        m_dual = &g_dual;
        // randomly chose first vertex
        m_curr_road = spawn_point;
        // randomly chose destination vertex and compute dijkstra shortest path
        double distance = 0;
        m_dest_road = dest_point;
        std::tie(m_path, distance) = boost::get_dijkstra_shortest_path(m_curr_road, m_dest_road, g_dual);
        // prints some info
        //DEBUG("Agents n. " + std::to_string(this->get_id()) + " born in road: " + std::to_string(g_dual[this->get_vertex()].index) + ", dest in " + std::to_string(g_dual[this->get_vertex_destination()].index) + " (d = " + std::to_string(distance) + ")");
    }

    //agent that do not move. Used for debug purpose
    Agent(Graph& g_dual, Vertex spawn_point, Vertex dest_point, std::map<Vertex, Edge> dual_map, int n_debug)
    {
        m_internal_time = 1;
        m_id = m_instances;
        m_instances++;
        m_dual_map = dual_map;
        m_dual = &g_dual;
        // randomly chose first vertex
        m_curr_road = spawn_point;
        // randomly chose destination vertex and compute dijkstra shortest path
        m_dest_road = dest_point;
        for (int i = 0; i < n_debug; i++) {
            m_path.push_back(m_curr_road);
        }
        // prints some info
        //DEBUG("Agents n. " + std::to_string(this->get_id()) + " born in road: " + std::to_string(this->get_vertex()) + ", dest in " + std::to_string(this->get_vertex_destination()) + " (d = " + std::to_string(distance) + ")");
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

    Vertex get_vertex_destination() const{
        return m_dest_road;
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

    Vertex get_next_vertex() const
    {
        return m_path[m_internal_time];
    }

};

#endif