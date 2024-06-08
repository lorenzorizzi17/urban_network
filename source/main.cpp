#include <iostream>
#include <fstream>
#include <boost/graph/graphviz.hpp>
#include "./headers/create_data.hpp"
#include "./headers/dual_graph.hpp"
#include "./headers/alias.hpp"
#include "./headers/dijkstra.hpp"
#include "./headers/agents.hpp"
#include "./headers/randomize.hpp"
#include "./headers/renderer.hpp"
#include "./headers/flow.hpp"

int Agent::m_instances = 0;


int main()
{
    Application
    srand(time(NULL));
    // N is the dimension of the urban network (number of crosspoints)
    const int N = 100;
    // creating manhattan data
    boost::create_manhattan_data(std::sqrt(N));

    // storing the graph based on previous operation on .txt file
    Graph g(N);
    std::ifstream input_file("data/manhattan_grid.dat");
    while (input_file.good())
    {
        int x, y;
        input_file >> x >> y;
        std::pair<Edge, bool> result = boost::add_edge(x, y, g);
    }

    // add a little bit of randomization
    boost::add_diagonal_roads<Graph, int>(g, 10, std::sqrt(N));
    boost::remove_random_edge<Graph, Iter_Edge>(g, 5, std::sqrt(N));
    // randomize edges weight
    boost::randomize_weight_map_uniform<Graph, Iter_Edge>(g, 5, 7); // as for now, uniform distribution. Maybe gaussian?
    // boost::print_weight_map<Graph, Iter_Edge>(g);

    // creating dual graph
    std::map<Vertex, Edge> dual_map;
    Graph dual = boost::make_dual_graph(g, dual_map);

    // store both graphs in a .dot extension, ready to be rendered
    std::ofstream file("fig/graph.dot");
    boost::write_graphviz(file, g);
    file.close();
    system("neato -Tpng fig/graph.dot -o fig/graph.png");
    std::ofstream file_dual("fig/graph_dual.dot");
    boost::write_graphviz(file_dual, dual);
    file_dual.close();
    system("fdp -Tpng fig/graph_dual.dot -o fig/graph_dual.png");
    // running OD simulation
    const int N_AGENTS = 1;
    const double MIN_D = 40;

    boost::add_agents(dual, N_AGENTS, MIN_D, dual_map);

    int time = 0;
    int TIME_MAX = 400;
    unsigned const display_height = 0.8 * sf::VideoMode::getDesktopMode().height;
    int const fps = 60;

    // SFML rendering stuff
    sf::RenderWindow window(sf::VideoMode(display_height, display_height), "Roundabout", sf::Style::Default);
    window.setFramerateLimit(fps);

    std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m";
    std::cout << "\x1b[31m" << "\n            STARTING SIMULATION...            \n"
              << "\x1b[0m";
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

        // remove agents who arrived at their destination
        //boost::remove_arrived_agents(dual);
        /* int size = agents.size();
        for (int i = size - 1; i >= 0; i--)
        {
            if (agents[i].arrived())
            {
                std::cout << "Agent n. " << agents[i].get_id() << " has arrived. ";
                agents.erase(agents.begin() + i);
                Agent a = Agent(dual, MIN_D, dual_map);
                agents.push_back(a);
            }
        } */

        
        // for every delta t, update agents position based on dijkstra shortest path
        std::for_each(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v){ boost::flow(v,dual,3); });
        
        // print info about current agent state
        /* for (auto it = agents.begin(); it != agents.end(); it++)
        {
            std::cerr << "Current agent position n. " << it->get_id() << " on the grid: " << it->get_road() << " at time  t= " << time << std::endl;
        } */
   

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