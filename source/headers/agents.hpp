//this file will implement the agent perspective in a OD model

#ifndef AGENTS
#define AGENTS


#include"./alias.hpp"
#include"./randomize.hpp"
#include"./dijkstra.hpp"

class Agent {
    private:
    Edge m_curr_edge;
    Edge m_dest_edge;
    Vertex m_curr_vertex;
    Vertex m_dest_vertex;
    std::vector<Vertex> m_path;
    int m_internal_time = 1;
    
    int m_id = 0;

    public:
    static int m_instances;

    Agent(Graph const& g, double MIN_DIST){
        m_internal_time = 1;
        m_id = m_instances;
        m_instances++;
        //randomly chose first vertex
        Vertex vi = *boost::get_random_vertex(g);
        m_curr_vertex = vi;
        //randomly chose destination vertex and compute dijkstra shortest path
        double distance = 0;
        std::tie(m_dest_vertex,m_path,distance) = boost::get_vertex_based_on_dijkstra_shortest_path(m_curr_vertex,g,MIN_DIST);

        //initialize edges
if (boost::edge(m_path[0],m_path[1],g).second){
        m_curr_edge = boost::edge(m_path[0],m_path[1],g).first;
}
if (boost::edge(m_path[m_path.size()-1],m_path[m_path.size()-2],g).second){
        m_dest_edge = boost::edge(m_path[m_path.size()-1],m_path[m_path.size()-2],g).first;
}
        //prints some info
        std::cout << "Agents n. " << this->get_id() << " born in node: " << this->get_vertex() << ", dest in " << this->get_vertex_destination() << "; from " << m_curr_edge << " to " << m_dest_edge << " (d = " << distance << ")" << std::endl; 
        
    }
    
    Vertex get_vertex() const {
        return m_curr_vertex;
    }

    Edge get_road() const {
        return m_curr_edge;
    }

    Vertex get_vertex_destination() const {
        return m_dest_vertex;
    }

    Edge get_road_destination() const {
        return m_dest_edge;
    }


    void set_edge(Edge const& e){
        m_curr_edge = e;
    }

    std::vector<Vertex> get_route() const{
        return m_path;
    }

    void evolve_dijsktra(Graph const& g){
        m_curr_vertex = m_path[m_internal_time];
if (boost::edge(m_path[m_internal_time],m_path[m_internal_time+1],g).second){
        m_curr_edge = boost::edge(m_path[m_internal_time],m_path[m_internal_time+1],g).first;
}
        m_internal_time++;
    }

    bool arrived() const{
        return m_curr_edge == m_dest_edge;
    }

    int get_id()const{  
        return m_id;
    }

    /* double get_distance_left(Graph const& g,Graph const& g_dual, std::map<Edge,Vertex>& map){
        return boost::get_dijkstra_shortest_path(m_curr_edge,m_dest_edge,g,g_dual, map).second;
    } */

};

#endif