#include <iostream>
#include<fstream>
#include<boost/graph/graphviz.hpp>
#include"./headers/create_data.hpp"
#include"./headers/dual_graph.hpp"
#include"./headers/alias.hpp"
#include"./headers/dijkstra.hpp"
#include"./headers/agents.hpp"
#include"./headers/randomize.hpp"
#include"./headers/renderer.hpp"

int Agent::m_instances = 0;

namespace std{
    template <typename T>
    std::istream& operator>>(std::istream& in, std::pair<T,T>& p) {
    in >> p.first >> p.second;
    return in;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out, std::pair<T,T> const& p) {
    out << p.first << " " << p.second;
    return out;
    }
};

int main() {
    srand(time(NULL));
    //N is the dimension of the urban network (number of crosspoints)
    const int N = 100;
    //creating manhattan data
    boost::create_manhattan_data(std::sqrt(N));

    //storing the grapg based on previous operation on .txt file
    Graph g(N);
    std::ifstream input_file("data/manhattan_grid.dat");
    while(input_file.good()){
        int x,y;
        input_file >> x >> y;
        std::pair<Edge, bool> result = boost::add_edge(x, y, g);
    }

    //add a little bit of randomization
    boost::add_diagonal_roads<Graph,int>(g,8,std::sqrt(N));
    boost::remove_random_edge<Graph,Iter_Edge>(g,5,std::sqrt(N));
    //randomize edges weight
    boost::randomize_weight_map_uniform<Graph, Iter_Edge>(g,5,7);    //as for now, uniform distribution. Maybe gaussian? 
    //boost::print_weight_map<Graph, Iter_Edge>(g);
    
    //creating dual graph
    std::map<Vertex, Edge> dual_map;
    Graph dual = boost::make_dual_graph(g,dual_map);

std::cerr << boost::source(dual_map[*boost::vertices(dual).first],g) << " " << boost::target(dual_map[*boost::vertices(dual).first],g);

    //store both graphs in a .dot extension, ready to be rendered
    std::ofstream file("fig/graph.dot");
    boost::write_graphviz(file, g);
    file.close();
    system("neato -Tpng fig/graph.dot -o fig/graph.png");
    std::ofstream file_dual("fig/graph_dual.dot");
    boost::write_graphviz(file_dual, dual);
    file_dual.close();
    system("fdp -Tpng fig/graph_dual.dot -o fig/graph_dual.png");

    //running OD simulation
    const int N_AGENTS = 100;
    const double MIN_D = 30;

    //declaring and defining agents
    std::vector<Agent> agents;
    for (int n = 0; n < N_AGENTS; n++)
    {
        Agent a = Agent(g,MIN_D);
        agents.push_back(a);
    }
    std::cout << std::endl;

    int time = 0;
    int TIME_MAX = 400;
    unsigned const display_height = 0.95 * sf::VideoMode::getDesktopMode().height; //=768
    int const fps = 60;

    //SFML rendering stuff
    sf::RenderWindow window(sf::VideoMode(display_height, display_height),"Roundabout", sf::Style::Default);
    window.setFramerateLimit(fps);

    //graphics loop
    while (window.isOpen() && time < TIME_MAX){

        //event handler
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
            window.close();
          }
        }
        window.clear(sf::Color::White);

       
        boost::render_graph(window,g,std::sqrt(N), agents,false);

        //for every delta t, update agents position based on dijkstra shortest path
        std::for_each(agents.begin(),agents.end(),[&](Agent& a){a.evolve_dijsktra(g);});
        for(auto it = agents.begin(); it != agents.end() ;it++){
            std::cerr << "Current agent position n. " << it->get_id() << " on the grid: " << it->get_vertex() << " at time  t= " << time << std::endl;     
        }

        //remove agents who arrived at their destination
        int size = agents.size();
        for (int i = size-1; i >= 0; i--)
        {
            if(agents[i].arrived()){
                std::cout << "Agent n. " << agents[i].get_id() << " has arrived. ";
                agents.erase(agents.begin()+i);
                Agent a = Agent(g,MIN_D); agents.push_back(a);
            }
        }
        
        time++;
        std::cout << std::endl;
        window.display();
        sleep(0.1);
    }       
}
