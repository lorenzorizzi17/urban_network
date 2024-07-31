#ifndef AGENTS
#define AGENTS

#include "./alias.hpp"


namespace rw
{
    class Agent
    {
    private:
        Vertex m_curr_road; 
        int m_internal_time = 1;
        int m_id = 0;
        Graph* m_dual;

    public:
        static int m_instances;
        bool m_trav = false;
        // std::map here is just for debug
        Agent(Graph& g_dual, Vertex spawn_point)
        {
            m_internal_time = 1;
            m_id = m_instances;
            m_instances++;
            m_dual = &g_dual;
            // randomly chose first vertex
            m_curr_road = spawn_point;
        }

        Vertex get_vertex() const
        {
            return m_curr_road;
        }

        void set_edge(Vertex const& v)
        {
            m_curr_road = v;
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

    Iter_Vertex get_random_vertex(Graph const& g) {
        Iter_Vertex vi = boost::vertices(g).first;
        int num_e = boost::num_vertices(g);
        std::advance(vi, std::rand() % num_e);
        //strip down from here
        while (get(&VertexProperty::queue, g, *vi).size() >= MAX_CAP) {
            vi = boost::vertices(g).first;
            int num_e = boost::num_vertices(g);
            std::advance(vi, std::rand() % num_e);
        }
        return vi;
    }


    void add_agents(Graph& g_dual, int N)
    {
        for (int i = 0; i < N; i++)
        {
            Vertex v = *get_random_vertex(g_dual);
            boost::get(&VertexProperty::queue, g_dual, v).push_back(std::make_shared<Agent>(g_dual, v));
        }
    }

    void add_agent(Graph& g_dual, Vertex v)
    {
            boost::get(&VertexProperty::queue, g_dual, v).push_back(std::make_shared<Agent>(g_dual, v));
  
    }

    
}

#endif