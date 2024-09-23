#ifndef AGENTS
#define AGENTS

#include<tuple>
#include"alias.hpp"
#include"config.hpp"

class Agent {
    private:
        Vertex m_curr_road; // vertex in dual graph = road
        Vertex m_dest_road;
        std::vector<Vertex> m_path;
        int m_internal_time = 1;
       
        int m_id = 0;
        int m_perm_time = 0;

        std::map<Vertex, Edge> m_dual_map;
        Graph* m_dual;

		static Chart m_chart;

    public:
        int m_total_time = 0;
        static int m_instances;
        static int m_existing_agent;
        bool m_trav = false;
        // std::map here is just for debug
        Agent(Graph&, Vertex, std::map<Vertex, Edge>, Config const&);
        ~Agent();

        Vertex get_vertex() const;
        Vertex get_vertex_destination() const;

        Edge get_road() const;
        Edge get_road_destination() const;
        std::vector<Vertex> get_route() const;

        void set_edge(Vertex const& v);

        void evolve_dijsktra();

        bool arrived() const;

        int get_perm_time() const;

        int get_id() const;

        bool has_traveled() const;

        void set_traveled(bool);

        Vertex get_next_vertex() const;

        void set_perm_time(int);

        static void initialize_chart(Graph const&);
};


#endif