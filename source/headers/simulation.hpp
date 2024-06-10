#ifndef SIMULATION
#define SIMULATION

#include "graph_builder.hpp"
#include "renderer.hpp"
#include "flow.hpp"

int Agent::m_instances = 0;

class Simulation
{
private:
    unsigned const display_height = 0.8 * sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow m_window;
    Graph m_graph;
    Graph m_dual;
    std::map<Vertex, Edge> m_conv_map;
    int m_time = 0;

private:
    void init()
    {
        // building graph and dual graph
        m_graph = build_graph(N_NODES);
        m_dual = boost::make_dual_graph(m_graph, m_conv_map);
        print_graph(m_graph, m_dual);
    }

public:
    Simulation(int N) : m_window{sf::RenderWindow(sf::VideoMode(display_height, display_height), "Simulation", sf::Style::Default)}
    {
        init();
        boost::add_agents(m_dual, N_AGENTS, m_conv_map);
        m_window.setFramerateLimit(60);
    }

    void run()
    {

        std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m";
        std::cout << "\x1b[31m" << "\n#                 STARTING SIMULATION...                     #\n"
                  << "\x1b[0m";
        std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m" << std::endl;

        while (m_window.isOpen())
        {
            // event handler
            sf::Event event;
            while (m_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    m_window.close();
                }
            }
            m_window.clear(sf::Color::White);

            // for every delta t, update agents position based on dijkstra shortest path
            std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                          {boost::flow(v, m_dual, FLOW_RATE);});
            std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                          {boost::erase_agents(v, m_dual);});
            std::for_each(boost::vertices(m_dual).first, boost::vertices(m_dual).second, [&](Vertex v)
                          {boost::print_info(v,m_dual,m_time,m_conv_map);});
                           
            // draw them
            boost::render_graph(m_window, m_graph, m_dual, m_conv_map, std::sqrt(N_NODES), true);

            // parsing p_ij
            boost::reactivate_flag(m_dual);
            m_time++;
            std::cout << std::endl;
            m_window.display();
            sf::sleep(sf::milliseconds(TIME_TO_SLEEP));
        }
    }

    //debug

    void printConversionMap() const {
        std::for_each(m_conv_map.begin(),m_conv_map.end(),[](std::pair<Vertex,Edge> const& pair){std::cout << "Vertex n. " << pair.first << " = " << pair.second << std::endl; });
    }

    void printWeights() {
        boost::print_weight_map(m_graph);
    }
};

#endif