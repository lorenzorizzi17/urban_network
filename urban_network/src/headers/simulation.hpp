#ifndef SIMULATION
#define SIMULATION

#include "graph_builder.hpp"
#include "renderer.hpp"
#include "flow.hpp"
#include "debugger.hpp"

#if DEBUG_MODE
#define DEBUG(x) DEBUG(x)
#else
#define DEBUG(x)
#endif

int Agent::m_instances = 0;

class Simulation
{
private:
    unsigned const display_height = 0.8*sf::VideoMode::getDesktopMode().height;
    std::unique_ptr<sf::RenderWindow> m_window;
    Graph m_graph;
    Graph m_dual;
    std::map<Vertex, Edge> m_conv_map;
    int m_time = 0;

private:
    void init()
    {
#if LOAD_GRAPH == true
        DEBUG("Loading graph...");
        load_graph("fig/graph.dot", m_graph);
        DEBUG("Direct graph has been successfully loaded into memory");
        DEBUG("Loading dual graph...");
        load_graph_dual("fig/graph_dual.dot",m_dual,m_graph,m_conv_map);
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
        
    }

public:

    Simulation(int N)
    {
        m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(display_height, display_height), "Simulation");
        DEBUG("Starting simulation construction...");
        init();
        DEBUG("Creating agents...");
        boost::add_agents(m_dual, N_AGENTS, m_conv_map);
        DEBUG("Agents have been correctly created");
        m_window->setFramerateLimit(60);
        DEBUG("Simulation is now ready to start");
    }

    void run(int ti)
    {
#ifdef _DEBUG
        std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m";
        std::cout << "\x1b[31m" << "\n#                 STARTING SIMULATION...                     #\n"
                  << "\x1b[0m";
        std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m" << std::endl;
#endif
        while (m_window->isOpen() && m_time < ti)
        {
            // event handler
            sf::Event event;
            while (m_window->pollEvent(event))
            {
                switch(event.type) {
                    case sf::Event::Closed:
                        m_window->close();
                        break;

                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::P) {
                            // Handle the 'P' key press
#if COLOR_DUAL
                            print_dual_paint(m_dual);

#endif
                        }
                        break;

                    default:
                        break;
                        }
            }

            m_window->clear(sf::Color::White);

            boost::increase_perm_time(m_dual);
            // for every delta t, update agents position based on dijkstra shortest path
            std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                          { boost::flow(v, m_dual, FLOW_RATE); });
            std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                          { boost::erase_agents(v, m_dual, m_conv_map); });
#ifdef _DEBUG
            {
                std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                              { boost::print_info(v, m_graph, m_dual, m_time, m_conv_map); });
            }
            std::cout << std::endl
                << std::endl;
#endif
            boost::render_graph(*m_window, m_graph, m_dual, m_conv_map, std::sqrt(N_NODES), true);
            // boost::thermal_shake(m_dual);
            //  draw them
            // boost::render_graph(m_window, m_graph, m_dual, m_conv_map, std::sqrt(N_NODES), true);

            // parsing p_ij
            boost::reactivate_flag(m_dual);

            m_time++;
            m_window->display();
            sf::sleep(sf::milliseconds(TIME_TO_SLEEP));
        }
    }

    // debug

    void printConversionMap() const
    {
        std::for_each(m_conv_map.begin(), m_conv_map.end(), [](std::pair<Vertex, Edge> const &pair)
                      { std::cout << "Vertex n. " << pair.first << " = " << pair.second << std::endl; });
    }

    void printWeights()
    {
        boost::print_weight_map(m_graph);
    }
};

#endif