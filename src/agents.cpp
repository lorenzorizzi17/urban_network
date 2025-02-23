#include"headers/agents.hpp"
#include "headers/dijkstra.hpp"


Agent::Agent(Graph& g_dual, Vertex spawn_point, std::map<Vertex, Edge> dual_map, Config const& conf)
{
    m_internal_time = 1;
    m_id = m_instances;
    m_instances++;
    m_perm_time = 0;
    m_dual_map = dual_map;
    m_dual = &g_dual;
    // randomly chose first vertex
    m_curr_road = spawn_point;
    // randomly chose destination vertex and compute dijkstra shortest path
    double distance = 0;
    std::tie(m_dest_road, m_path, distance) = get_vertex_based_on_dijkstra_shortest_path(m_curr_road, g_dual, conf, Agent::m_chart);
    m_existing_agent++;
}

Agent::~Agent() {
    m_existing_agent--;
}

Vertex Agent::get_vertex() const
{
    return m_curr_road;
}

Vertex Agent::get_vertex_destination() const {
    return m_dest_road;
}

std::vector<Vertex> Agent::get_route() const
{
    return m_path;
}

Edge Agent::get_road() const
{
    return m_dual_map.at(m_curr_road);
}

Edge Agent::get_road_destination() const
{
    return m_dual_map.at(m_dest_road);
}

void Agent::set_edge(Vertex const& v)
{
    m_curr_road = v;
}

void Agent::evolve_dijsktra()
{
    m_curr_road = m_path[m_internal_time];
    m_internal_time++;
    m_trav = true;
    m_perm_time = 0;
}

bool Agent::arrived() const
{
    return m_curr_road == m_dest_road;
}

int Agent::get_perm_time() const {
    return m_perm_time;
}

int Agent::get_id() const
{
    return m_id;
}

bool Agent::has_traveled() const
{
    return m_trav;
}

void Agent::set_traveled(bool b)
{
    m_trav = b;
}

Vertex Agent::get_next_vertex() const
{
    return m_path[m_internal_time];
}

void Agent::set_perm_time(int n) {
    m_perm_time = n;
}

void Agent::initialize_chart(Graph const& g)
{
    Agent::m_chart = create_static_chart(g);
}