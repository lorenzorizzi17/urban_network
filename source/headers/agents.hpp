//this file will implement the agent perspective in a OD model
#include"./alias.hpp"

class Agent {
    private:
    Vertex m_curr_vertex;
    Vertex m_dest_vertex;

    public:
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

};