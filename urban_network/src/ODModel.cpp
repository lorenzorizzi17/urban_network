#include"headers/ODModel.hpp"
#include"headers/agents.hpp"
#include"headers/parameters.hpp"
#include"headers/gridlock_exception.hpp"
#include"headers/graph_utilities.hpp"

int Agent::m_instances = 0;

//initialize the simulation
void ODModel::init(){
    if (LOAD_GRAPH) {
        DEBUG("Loading graph...");
        load_graph("graph/graph.dot", m_graph);
        DEBUG("Direct graph has been successfully loaded into memory");
        DEBUG("Loading dual graph...");
        load_graph_dual("graph/graph_dual.dot", m_dual, m_graph, m_conv_map);
        DEBUG("Dual graph has been successfully loaded into memory");
    } else {
        DEBUG("Building graph...");
        m_graph = build_graph(N_NODES);
        DEBUG("Direct graph has been successfully built");
        DEBUG("Building dual graph...");
        m_dual = make_dual_graph(m_graph, m_conv_map);
        DEBUG("Successfully built dual graph");
        DEBUG("Drawing graphs...");
        print_graph(m_graph, m_dual, m_conv_map);
        DEBUG("Direct and dual graph have been correctly drawn");
    }
    DEBUG("Creating parser graph...");
    m_parser = make_parser(m_dual);
    DEBUG("Parser graph correctly built.");
}

void ODModel::add_agents(int n_agents)
{
    for (int i = 0; i < n_agents; i++)
    {
        Vertex v = *get_random_vertex(m_dual);
        boost::get(&VertexProperty::queue, m_dual, v).push_back(std::make_shared<Agent>(m_dual, v, m_conv_map));
    }
}

ODModel::ODModel(int N) : m_stats( std::vector<int>{LOG_OCCUPATION_VS_TIME_NODE}, N){
    DEBUG("Starting simulation construction...");
    init();
    DEBUG("Creating agents...");
    add_agents(N);
    m_N = N;
    DEBUG("Agents have been correctly created");

    if (ENABLE_GRAPHICS) {
        DEBUG("Creating windows...");
        m_main_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(display_height, display_height), "Simulation");
        m_stats_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(0.4 * display_height, 0.16 * display_height), "Stats", sf::Style::Resize);
        m_graph_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(0.6 * display_height, 0.4 * display_height), ("Real-time graph (node " + std::to_string(LOG_OCCUPATION_VS_TIME_NODE) + ")").c_str());
        m_histo_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(0.6 * display_height, 0.4 * display_height), "Flow histogram");
        m_histo_post_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(0.6 * display_height, 0.4 * display_height), "RT histogram");
        m_main_window->setFramerateLimit(60);
        m_stats_window->setFramerateLimit(60);
        m_graph_window->setFramerateLimit(60);
        m_histo_window->setFramerateLimit(60);
        m_histo_post_window->setFramerateLimit(60);
        DEBUG("Windows created.");
    }
    DEBUG("Simulation is now ready to start");
}

ODModel::~ODModel() {
    //at destruction time, parse the graph
    if(PARSING_MODE){
        //build_parser(m_parser, m_N);
    }
    //save the statistics data
	if (PROCESS_STATS) {
		m_stats.save(m_dual);
	}
}

void ODModel::run_graphics(int time_max) {
    //display stuff
    #ifdef _DEBUG
    std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m";
    std::cout << "\x1b[31m" << "\n#                 STARTING SIMULATION...                     #\n" << "\x1b[0m";
    std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m" << std::endl;
    #endif

    //sim loop
    while (m_main_window->isOpen() && m_time < time_max)
    {
        //event handler
        handle_events(m_main_window);

        //clear windows
        m_stats_window->clear(sf::Color::White);
        m_main_window->clear(sf::Color::White);

        //setup statistics panel
        m_stats.clear();

        //dynamic evolution: flow, update weights, erase agents
        BGL_FORALL_VERTICES(v, m_dual, Graph) { flow(v, FLOW_RATE); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { update_weights(v); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { erase_agents(v); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { set_flag(v); }

        //if we want to process stats, update data          
        if (PROCESS_STATS) {
            m_stats.update(m_dual);
            m_stats.display_data(m_graph_window, m_histo_window, m_histo_post_window, m_time);
        }

        //draws the outcome
        render_graph(*m_main_window, m_graph, m_dual, m_conv_map, std::sqrt(N_NODES));

        //stops the simulation if a hard gridlock is reached
        check_for_gridlock(m_dual, m_time);

        //display SFML windows
        set_stats_text(display_height, m_time, m_dual, m_stats_window);
        m_stats_window->display();
        m_main_window->display();

        #if TIME_TO_SLEEP
        sf::sleep(sf::milliseconds(TIME_TO_SLEEP));
        #endif

        m_time++;
    }
}


void ODModel::run(int time_max) {
    while (m_time < time_max){
        //setup statistics panel
        m_stats.clear();

        //dynamical rule: flow, update weights, erase agents
        BGL_FORALL_VERTICES(v, m_dual, Graph) { flow(v, FLOW_RATE); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { update_weights(v); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { erase_agents(v); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { set_flag(v);}

        m_stats.update(m_dual);

        //stops the simulation if a gridlock is reached
        check_for_gridlock(m_dual, m_time);

        m_time++;
    }
}

