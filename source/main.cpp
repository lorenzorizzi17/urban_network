#define DIST_MIN 40

#include"./headers/graph_builder.hpp"
#include "./headers/dijkstra.hpp"
#include "./headers/agents.hpp"
#include "./headers/randomize.hpp"
#include "./headers/renderer.hpp"
#include "./headers/flow.hpp"

int Agent::m_instances = 0;


int main()
{
    const int N = 100;
    const int N_AGENTS = 200;
    const double MIN_D = 40; 

    //building graph and dual graph
    Graph g = build_graph(N);
    std::map<Vertex, Edge> dual_map;
    Graph dual = boost::make_dual_graph(g, dual_map);
    print_graph(g,dual);


    // running OD simulation
    boost::add_agents(dual, N_AGENTS, MIN_D, dual_map);

    int time = 0;
    int TIME_MAX = 400;
    unsigned const display_height = 0.8 * sf::VideoMode::getDesktopMode().height;
    int const fps = 60;

    // SFML rendering stuff
    sf::RenderWindow window(sf::VideoMode(display_height, display_height), "Roundabout", sf::Style::Default);
    window.setFramerateLimit(fps);

    std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m";
    std::cout << "\x1b[31m" << "\n#                 STARTING SIMULATION...                     #\n" << "\x1b[0m";
    std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m" << std::endl;

    while (window.isOpen() && time < TIME_MAX)
    {
        // event handler
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color::White);


        
        // for every delta t, update agents position based on dijkstra shortest path
        std::for_each(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v){ boost::flow(v,dual,3); });

        std::for_each(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v){ boost::erase_agents(v,dual); });

        std::for_each(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v){ boost::print_info(v,dual,time,dual_map); });
        // draw them
        boost::render_graph(window, g,dual, dual_map, std::sqrt(N), false);

    
        // parsing p_ij
        boost::reactivate_flag(dual);
        time++;
        std::cout << std::endl;
        window.display();
        sf::sleep(sf::milliseconds(1000));
    }
}