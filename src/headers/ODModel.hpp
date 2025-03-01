#ifndef OD_SIMULATION
#define OD_SIMULATION

#include<numeric>

#include "graph_builder.hpp"
#include "parser.hpp"
#include "graphics.hpp"
#include "config.hpp"
#include "statistics.hpp"
#include "omp.h"

#if _DEBUG
#define DEBUG(x) DEBUG(x)
#else
#define DEBUG(x)
#endif

class ODModel{
    public:
        ODModel();
        ODModel(int);
        ~ODModel();
        void run_graphics();
        void run();

    private:
        void init();
        void add_agents(int);
		void add_agents();
        void flow(Vertex, int);
        void flow_multithreading(Vertex, int, std::vector<omp_lock_t>&);
		void update_weights(Vertex);
		void erase_agents(Vertex);
        void set_flag(Vertex);
		void render_graph(sf::RenderWindow&, Graph const&, Graph const&, std::map<Vertex, Edge>&);
        void render_stat_panel(sf::RenderWindow&, Config const&, int, int);

    private:
        unsigned const display_height = 0.79 * sf::VideoMode::getDesktopMode().height;
        std::unique_ptr<sf::RenderWindow> m_main_window;

        Graph m_graph;
        Graph m_dual;
        Parser m_parser;

        Config m_config;
        Statistics m_stats;

        std::map<Vertex, Edge> m_conv_map;

        int m_time = 0;
        bool m_is_running = true;
};

#endif