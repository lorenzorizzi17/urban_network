#ifndef RW_SIMULATION
#define RW_SIMULATION

#include"agents.hpp"
#include"graph_builder.hpp"
#include"flow.hpp"
#include"renderer.hpp"
#include"printers.hpp"
#include"stats.hpp"
#include<SFML/Graphics.hpp>

#if _DEBUG
#define DEBUG(x) DEBUG(x)
#else
#define DEBUG(x)
#endif

int rw::Agent::m_instances = 0;

namespace rw {

    class RWModel
    {
    private:
        unsigned const display_height = 0.8 * sf::VideoMode::getDesktopMode().height;
        std::unique_ptr<sf::RenderWindow> m_main_window;
        std::unique_ptr<sf::RenderWindow> m_stats_window;
        Graph m_graph;
        Graph m_dual;
        std::map<Vertex, Edge> m_conv_map;

        Statistics m_stats;

        int m_time = 0;
        std::vector<int> m_flux;
        int m_N;
		double m_perc = 0.0;
    private:
        void init(int n)
        {
            DEBUG("Loading graph...");
            load_graph("fig/graph.dot", m_graph);
            DEBUG("Direct graph has been successfully loaded into memory");
            DEBUG("Loading dual graph...");
            load_dual("fig/parser/parser_N2000.dot", m_dual); //load_dual("fig/parser/parser_N"+std::to_string(n) + ".dot", m_dual);
            DEBUG("Dual graph has been successfully loaded into memory");
        }

    public:

		RWModel(int n, double perc) : m_stats(n,0)//m_stats({ 10,330,262,205,158,209,252,187,355,0 }, n)
        {
            DEBUG("Starting simulation construction...");
            init(n);
            DEBUG("Creating agents...");
            add_agents(m_dual, n);
            DEBUG("Agents have been correctly created");
            DEBUG("Creating windows...");
            m_N = n;
			m_perc = perc;
            m_main_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(display_height, display_height), "Simulation");
            m_stats_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(0.4 * display_height, 0.16 * display_height), "Stats", sf::Style::Resize);
            m_main_window->setFramerateLimit(60);
            m_stats_window->setFramerateLimit(60);
            DEBUG("Windows created.");
            DEBUG("Simulation is now ready to start");
        }

        ~RWModel() {
            #if PARSE_MODE
            print_mean_congestion(m_dual, m_time, m_N, m_perc);
            #endif
        }

        

        void run(int time_max)
        {
            while (m_main_window->isOpen() && m_time < time_max)
            {
                // event handler
                sf::Event event;
                while (m_main_window->pollEvent(event))
                {
                    switch (event.type) {
                    case sf::Event::Closed:
                        m_main_window->close();
                        break;
                    default:
                        break;
                    }
                }
                //clear windows
                m_stats_window->clear(sf::Color::White);
                m_main_window->clear(sf::Color::White);

                //setup statistics panel
                m_stats.clear();

                // DYNAMICAL RULE
                BGL_FORALL_VERTICES(v, m_dual, Graph) { flow(v, m_dual, FLOW_RATE, m_stats);}
             
         

                //draws the outcome
                #if GRAPHICS
                render_graph(*m_main_window, m_graph, m_dual, std::sqrt(N_NODES));
                #endif

                //set flag (congested nodes)
                this->set_flags();
                
                m_stats.update(m_dual);
                
                //update_queue_size(o, m_dual,m_time, 1);

                //display stuff
                m_time++;
                m_main_window->display();
                m_stats_window->display();

                

                sf::sleep(sf::milliseconds(TIME_TO_SLEEP));

				//if TIME_MAX_SIMULATION divided by m_time is a multiple of 10, print the mean congestion
				if (m_time%(100) == 0) {
                    std::cout << m_time / 100 << " " ;
                }

            }
        }

        void set_flags() {
            //needed to reactivate traveled cars
            reactivate_flag(m_dual);
            //set nodes as congestionated 
            set_flag_vertices(m_dual);
        }

        void save(int run) {
            m_stats.save(m_dual);
        }

        

    };
}

#endif