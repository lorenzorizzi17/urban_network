#ifndef OD_SIMULATION
#define OD_SIMULATION

#include<numeric>

#include "graph_builder.hpp"
#include "parser.hpp"
#include "graphics.hpp"
#include "config.hpp"
#include "statistics.hpp"

#if _DEBUG
#define DEBUG(x) DEBUG(x)
#else
#define DEBUG(x)
#endif

class ODModel{
    public:
        ODModel();
        ~ODModel();

        void run_graphics();
        void run();

    private:
        void init();
        void add_agents(int);
        void flow(Vertex, int);
		void update_weights(Vertex);
		void erase_agents(Vertex);
        void set_flag(Vertex);
		void render_graph(sf::RenderWindow&, Graph const&, Graph const&, std::map<Vertex, Edge>&);
        void render_stat_panel(sf::RenderWindow&, Config const&, int);

    private:
        unsigned const display_height = 0.8 * sf::VideoMode::getDesktopMode().height;

        std::unique_ptr<sf::RenderWindow> m_main_window;
        std::unique_ptr<sf::RenderWindow> m_stats_window;

        Config m_config;

        Graph m_graph;
        Graph m_dual;
        Parser m_parser;
        std::map<Vertex, Edge> m_conv_map;
        Statistics m_stats;

        int m_time = 0;
        int m_N;


};

#endif