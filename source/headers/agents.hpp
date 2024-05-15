//this file will implement the agent perspective in a OD model
#include"./alias.hpp"
#include"./dijkstra.hpp"

class Agent {
    private:
    Vertex m_curr_vertex;
    Vertex m_dest_vertex;
    std::vector<Vertex> m_path;
    int m_internal_time = 0;
    
    int m_id = 0;

    public:
    static int m_instances;
    Agent(Vertex const& vin, Vertex const& vfi, Graph const& g) : m_curr_vertex{vin}, m_dest_vertex{vfi} {};
    Agent(int id_v, int id_f, Graph const& g){
        IdMap id_map = boost::get(boost::vertex_index, g);
        m_curr_vertex = id_map[id_v];
        m_dest_vertex = id_map[id_f];
    }
    //random spawning
    Agent(Graph const& g){
        //take a random vertex
        Iter_Vertex iti = boost::vertices(g).first;
        int rnd = std::rand() % (boost::num_vertices(g));
        std::advance(iti, rnd);
        m_curr_vertex = *iti;
        iti = boost::vertices(g).first;
        rnd = std::rand() % (boost::num_vertices(g));
        std::advance(iti, rnd);
        m_dest_vertex = *iti;
        //now builds the shortest dijkstra path
        m_internal_time = 0;
        m_id = m_instances;
        m_path = boost::get_dijkstra_shortest_path(m_curr_vertex,m_dest_vertex,g);
        m_instances++;
    }

    Vertex get_vertex() const {
        return m_curr_vertex;
    }

    Vertex get_destination() const {
        return m_dest_vertex;
    }

    void set_vertex(Vertex const& v){
        m_curr_vertex = v;
    }

    std::vector<Vertex> get_route() const{
        return m_path;
    }

    void evolve_dijsktra(){
        if(m_curr_vertex != m_dest_vertex){
            m_curr_vertex = m_path[m_internal_time];
        }
        m_internal_time++;
    }

    bool arrived() const{
        return m_curr_vertex == m_dest_vertex;
    }

    int get_id()const{  
        return m_id;
    }

};