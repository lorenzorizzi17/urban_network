#ifndef OD_SIMULATION
#define OD_SIMULATION

#include<numeric>

#include "graph_builder.hpp"
#include "renderer.hpp"
#include "flow.hpp"
#include "debugger.hpp"
#include "parser.hpp"
#include "graphics.hpp"
#include "printers.hpp"
#include "stall.hpp"
#include "stats.hpp"


#if _DEBUG
#define DEBUG(x) DEBUG(x)
#else
#define DEBUG(x)
#endif

int od::Agent::m_instances = 0;

namespace od {

    class ODModel
    {
    private:
        unsigned const display_height = 0.8 * sf::VideoMode::getDesktopMode().height;
        std::unique_ptr<sf::RenderWindow> m_main_window;
        std::unique_ptr<sf::RenderWindow> m_stats_window;
        Graph m_graph;
        Graph m_dual;
        Parser m_parser;
        std::map<Vertex, Edge> m_conv_map;
        Statistics m_stats;

        int m_time = 0;

    private:
        void init()
        {
            #if LOAD_GRAPH == true
            DEBUG("Loading graph...");
            load_graph("fig/graph.dot", m_graph);
            DEBUG("Direct graph has been successfully loaded into memory");
            DEBUG("Loading dual graph...");
            load_graph_dual("fig/graph_dual.dot", m_dual, m_graph, m_conv_map);
            DEBUG("Dual graph has been successfully loaded into memory");
            #else
            DEBUG("Building graph...");
            m_graph = build_graph(N_NODES);
            DEBUG("Direct graph has been successfully built");
            DEBUG("Building dual graph...");
            m_dual = boost::make_dual_graph(m_graph, m_conv_map);
            DEBUG("Successfully built dual graph");
            DEBUG("Drawing graphs...");
            print_graph(m_graph, m_dual, m_conv_map);
            DEBUG("Direct and dual graph have been correctly drawn");
            #endif

            DEBUG("Creating parser graph...");
            m_parser = make_parser(m_dual);
            DEBUG("Parser graph correctly built.");
            
            
            

        }

    public:

        ODModel(int N) : m_stats({10,330,262,205,158,209,252,187,355,0}, N)
        {
            DEBUG("Starting simulation construction...");
            init();
            DEBUG("Creating agents...");
            add_agents(m_dual, N, m_conv_map);
            auto it = std::find_if(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v) {return m_dual[v].queue.size() > MAX_CAP; });
            if (it != boost::vertices(m_dual).second) {
                std::cout << "!";
            }
            DEBUG("Agents have been correctly created");
            DEBUG("Creating windows...");
            #if ENABLE_GRAPHICS
            m_main_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(display_height, display_height), "Simulation");
            m_stats_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(0.4 * display_height, 0.16 * display_height), "Stats", sf::Style::Resize);
            m_main_window->setFramerateLimit(60);
            m_stats_window->setFramerateLimit(60);
            #endif
            DEBUG("Windows created.");
            DEBUG("Simulation is now ready to start");
        }

        ~ODModel() {
        //algorithm to perform once the sim stops
        #if DRAW_CONGESTED == true
            print_mean_occupation(m_dual);
            print_percolation_graph(m_dual, m_time);
        #endif
        #if PARSING_MODE == true
            build_parser(m_parser);
        #endif 
        }


        void run_graphics(int time_max)
        {
            #ifdef _DEBUG
            std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m";
            std::cout << "\x1b[31m" << "\n#                 STARTING SIMULAtiON...                     #\n"
                << "\x1b[0m";
            std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m" << std::endl;
            #endif
            

            while (m_main_window->isOpen() && m_time < time_max)
            {
                // event handler
                handle_events(m_main_window, m_dual);
                //clear windows
                m_stats_window->clear(sf::Color::White);
                m_main_window->clear(sf::Color::White);

               
                //setup statistics panel
                m_stats.clear();

                //dynamic evolution
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { flow(v, m_dual, FLOW_RATE, m_parser, m_stats); });
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { update_weights(m_dual); });
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { erase_agents(v, m_dual, m_conv_map, m_stats); });

                #if PROCESS_STATS
                m_stats.update(m_dual);
                #endif

                #ifdef _DEBUG
                DEBUG("#### time t = " + std::to_string(m_time));
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { od::print_info(v, m_graph, m_dual, m_time, m_conv_map); });
                #endif

                //draws the outcome
                boost::render_graph(*m_main_window, m_graph, m_dual, m_conv_map, std::sqrt(N_NODES), true);
  

                //set flag (congested nodes)
                this->set_flags();
                //stops the simulation if a gridlock is reached
                d_check_for_gridlock_b(m_dual, m_time);
                if (m_time == TIME_MAX_SIMULATION-1) {
                    d_check_for_partial_gridlock_b(m_dual, m_time);
                }
               
                

                //display stuff
                m_time++;
                m_main_window->display();

                set_stats_text(display_height, m_time, m_dual, m_stats_window);
                m_stats_window->display();

                
                #if TIME_TO_SLEEP
                sf::sleep(sf::milliseconds(TIME_TO_SLEEP));
                #endif

            }
        }


        void run(int time_max) {
            while (m_time < time_max)
            {
                
                //setup statistics panel
                m_stats.clear();

                // DYNAMICAL RULE
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { flow(v, m_dual, FLOW_RATE, m_parser, m_stats); });
                //m_flux.push_back(flux);
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { update_weights(m_dual); });
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { erase_agents(v, m_dual, m_conv_map,m_stats); });
                #ifdef _DEBUG
                DEBUG("#### time t = " + std::to_string(m_time));
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                    { od::print_info(v, m_graph, m_dual, m_time, m_conv_map); });
                #endif

                m_stats.update(m_dual);


                //set flag (congested nodes)
                this->set_flags();


                //stops the simulation if a gridlock is reached
                d_check_for_gridlock_b(m_dual, m_time);
                if (m_time == TIME_MAX_SIMULATION - 1) {
                    d_check_for_partial_gridlock_b(m_dual, m_time);
                }

                m_time++;
            }
        }


        void save(int run) {
            #if PROCESS_STATS == true
            m_stats.save(m_dual);
            #endif
        }

        void set_flags() {
            //needed to reactivate traveled cars
            reactivate_flag(m_dual);
            //set nodes as congestionated 
            set_flag_vertices(m_dual);
        }

        void print_partial_gridlock() {
            get_stall_graph(m_dual);

        }

        void print_total_gridlock() {
            build_stall_graph(m_dual);
        }
        
    };
}

#endif