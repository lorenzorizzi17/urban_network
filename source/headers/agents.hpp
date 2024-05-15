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
    Agent(Graph const& g, double MIN_DIST){
        m_internal_time = 0;
        m_id = m_instances;
        m_instances++;
        //randomly chose first vertex
        Iter_Vertex iti = boost::vertices(g).first;
        std::advance(iti, std::rand() % (boost::num_vertices(g)));
        m_curr_vertex = *iti;
        //randomly chose destination vertex
        iti = boost::vertices(g).first;
        std::advance(iti, std::rand() % (boost::num_vertices(g)));
        double d = boost::get_dijkstra_shortest_path(m_curr_vertex,*iti,g).second;
        while(d < MIN_DIST){
            iti = boost::vertices(g).first;
            std::advance(iti, std::rand() % (boost::num_vertices(g)));
            d = boost::get_dijkstra_shortest_path(m_curr_vertex,*iti,g).second;
        }
        m_dest_vertex = *iti;
        m_path = boost::get_dijkstra_shortest_path(m_curr_vertex,m_dest_vertex,g).first;

        //prints some info
        std::cout << "Agents n. " << this->get_id() << " born in " << this->get_vertex() << ", dest in " << this->get_destination() << " (d = " << this->get_distance_left(g) << ")" << std::endl; 
        
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

    double get_distance_left(Graph const& g){
        return boost::get_dijkstra_shortest_path(m_curr_vertex,m_dest_vertex,g).second;
    }

};