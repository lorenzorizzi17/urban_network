#include"headers/ODModel.hpp"
#include"headers/agents.hpp"
#include"headers/gridlock_exception.hpp"
#include"headers/graph_utilities.hpp"
#include <omp.h>

int Agent::m_instances = 0;
int Agent::m_existing_agent = 0;
Chart Agent::m_chart;

//initialize the simulation
void ODModel::init(){
    #pragma omp critical
	    m_config.load_from_file("src/config.txt");
    
    if (m_config.LOAD_GRAPH) {
        DEBUG("Loading graph...");
        load_graph("graph/graph.dot", m_graph);
        DEBUG("Direct graph has been successfully loaded into memory");
        DEBUG("Loading dual graph...");
        load_graph_dual("graph/graph_dual.dot", m_dual, m_graph, m_conv_map);
        DEBUG("Dual graph has been successfully loaded into memory");
    } else {
        DEBUG("Building graph...");
        m_graph = build_graph(m_config);
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
    Agent::initialize_chart(m_dual);

}

void ODModel::add_agents(int n_agents)
{
    for (int i = 0; i < n_agents; i++)
    {
        Vertex v = *get_random_vertex(m_dual);
        boost::get(&VertexProperty::queue, m_dual, v).push_back(std::make_shared<Agent>(m_dual, v, m_conv_map, m_config));
    }
}

void ODModel::add_agents() {
	if (m_config.PROBABILITY != 0) {
		std::random_device rd;
		std::mt19937 gen(rd());
        std::poisson_distribution<> dis(m_config.PROBABILITY);
        add_agents(dis(gen));
	}
}

ODModel::ODModel() : m_stats( std::vector<int>{m_config.LOG_OCCUPATION_VS_TIME_NODE}){
    DEBUG("Starting simulation construction...");
    init();
    DEBUG("Creating agents...");
    add_agents(m_config.N_AGENTS_INITIAL);
    DEBUG("Agents have been correctly created");
    DEBUG("Simulation is now ready to start");
}

ODModel::ODModel(int N_AGENTS) : m_stats( std::vector<int>{m_config.LOG_OCCUPATION_VS_TIME_NODE}){
    DEBUG("Starting simulation construction...");
    init();
    DEBUG("Creating agents...");
    m_config.N_AGENTS_INITIAL = N_AGENTS;
    add_agents(m_config.N_AGENTS_INITIAL);
    DEBUG("Agents have been correctly created");
    DEBUG("Simulation is now ready to start");
}

ODModel::ODModel(int N_AGENTS, int TIME) : m_stats( std::vector<int>{m_config.LOG_OCCUPATION_VS_TIME_NODE}){
    DEBUG("Starting simulation construction...");
    init();
    DEBUG("Creating agents...");
    m_config.N_AGENTS_INITIAL = N_AGENTS;
    m_config.TIME_MAX_SIMULATION = TIME;
    add_agents(m_config.N_AGENTS_INITIAL);
    DEBUG("Agents have been correctly created");
    DEBUG("Simulation is now ready to start");
}

ODModel::~ODModel() {
    //at destruction time, parse the graph
    if(m_config.PARSING_MODE){
        //build_parser(m_parser, m_N);
    }
    //save the statistics data
	if (m_config.WRITE_DATA) {
		m_stats.save(m_dual, m_config.TIME_MAX_SIMULATION, m_config.N_AGENTS_INITIAL);
	}
}

void ODModel::run_graphics() {

    DEBUG("Creating windows...");
    m_main_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(2 * display_height, display_height), "Simulation");
    m_main_window->setFramerateLimit(120);
    DEBUG("Windows created.");

    //display stuff
    #ifdef _DEBUG
    std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m";
    std::cout << "\x1b[31m" << "\n#                 STARTING SIMULATION...                     #\n" << "\x1b[0m";
    std::cout << "\x1b[31m" << "################################################################" << "\x1b[0m" << std::endl;
    #endif

    sf::Clock clock;
    bool m_step = false;
    //sim loop
    while (m_main_window->isOpen() && m_time < m_config.TIME_MAX_SIMULATION)
    {
        
        sf::Time sftime = clock.restart();

		//add agents with a certain probability
        add_agents();

        //event handler
        handle_events(m_main_window, m_is_running, m_dual, m_step);

        //clear window
        m_main_window->clear(sf::Color(20,20,20,50));

        //setup statistics panel
        m_stats.clear();
        
        
        if (m_is_running || m_step) {
            //dynamic evolution: flow, update weights, erase agents
            BGL_FORALL_VERTICES(v, m_dual, Graph) { flow(v, m_config.FLOW_RATE); }
            BGL_FORALL_VERTICES(v, m_dual, Graph) { update_weights(v); }
            BGL_FORALL_VERTICES(v, m_dual, Graph) { erase_agents(v); }
            BGL_FORALL_VERTICES(v, m_dual, Graph) { set_flag(v); }
        }

        //draws the simulation on the appropriate panel
        render_graph(*m_main_window, m_graph, m_dual, m_conv_map);
        //draw the statistics panel
        render_stat_panel(*m_main_window, m_config,int(1 / sftime.asSeconds()), Agent::m_existing_agent);

        //if we want to process stats, update data          
        if (m_config.PROCESS_STATS) {
            m_stats.update(m_dual);
            m_stats.display_data(m_main_window, m_time, m_config);
        }

        //stops the simulation if a hard gridlock is reached
        check_for_gridlock(m_dual, m_time);

        //display SFML windows
        m_main_window->display();

        if(m_config.TIME_TO_SLEEP){
            sf::sleep(sf::milliseconds(m_config.TIME_TO_SLEEP));
        }

        m_time++;
    }
}


void ODModel::run(){
    #pragma omp critical
    std::cout << "Agents: " << m_config.N_AGENTS_INITIAL << ", T = " << m_config.TIME_MAX_SIMULATION << std::endl;
    while (m_time < m_config.TIME_MAX_SIMULATION){

        //setup statistics panel
        m_stats.clear();

        //dynamical rule: flow, update weights, erase agents
        BGL_FORALL_VERTICES(v, m_dual, Graph) { flow(v, m_config.FLOW_RATE); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { update_weights(v); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { erase_agents(v); }
        BGL_FORALL_VERTICES(v, m_dual, Graph) { set_flag(v); }

        m_stats.update(m_dual);

        //stops the simulation if a gridlock is reached
        check_for_gridlock(m_dual, m_time);

        m_time++;
    }
}