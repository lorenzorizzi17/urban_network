#include<numeric>

#include"alias.hpp"
#include"agents.hpp"
#include"stats.hpp"


namespace rw {

    void reactivate_flag_v(Vertex v, Graph& g)
    {
        std::list<std::shared_ptr<Agent>> ref = boost::get(&VertexProperty::queue, g, v);
        if (!ref.empty()) {
            std::for_each(ref.begin(), ref.end(), [](std::shared_ptr<Agent>& ptr_a)
                { ptr_a->set_traveled(false); });
        }
    }

    void reactivate_flag(Graph& g)
    {
        std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v)
            { reactivate_flag_v(v, g); });
    }

    void set_flag_vertices(Graph& g) {
        for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
            if (get(&VertexProperty::queue, g, *it).size() >= MAX_CAP) {
                get(&VertexProperty::full, g, *it) = true;
                get(&VertexProperty::congested_time, g,*it) += 1;
            }
            else {
                get(&VertexProperty::full, g, *it) = false;
            }
            get(&VertexProperty::congestion_time, g, *it) += get(&VertexProperty::queue, g, *it).size();
        }
    }

    std::pair<Vertex, bool> process_agent(Vertex v, Graph& g) {
        // Definizione del generatore di numeri casuali
        static std::random_device rd; // Fonte di entropia
        static std::mt19937 gen(rd()); // Generatore di numeri casuali
        std::uniform_real_distribution<> dis(0.0, 1.0); // Distribuzione tra 0 e 1

        double rn = dis(gen); // Genera un numero casuale tra 0 e 1
        double cum_p = 0;

        for (auto it2 = boost::out_edges(v, g).first; it2 != boost::out_edges(v, g).second; it2++) {
            cum_p += get(&EdgeProperty::prob, g, *it2);
            if (rn < cum_p) {
                Vertex v_tar = boost::target(*it2, g);
                if (get(&VertexProperty::full, g, v_tar)) {
                    return std::make_pair(0, false);
                }
                else {
                    return std::make_pair(v_tar, true);
                }
            }
        }

        // Se non è stato raggiunto un valore di 1, scegli una strada casuale
        std::cout << "Miss";
        std::uniform_int_distribution<> dis_int(0, boost::out_degree(v, g) - 1); // Distribuzione intera
        int rr = dis_int(gen);

        auto ch = boost::out_edges(v, g).first;
        std::advance(ch, rr);
        Vertex v_tar = boost::target(*ch, g);
        if (get(&VertexProperty::full, g, v_tar)) {
            return std::make_pair(0, false);
        }
        else {
            return std::make_pair(v_tar, true);
        }
    }

    void flow(Vertex v, Graph& g, int n, Statistics& s) {

            std::list<std::shared_ptr<rw::Agent>>& queue = get(&VertexProperty::queue, g, v);
            if (queue.empty()) {
                return;
            }

            int c = 0;
            auto it = queue.begin();
            std::pair<Vertex, bool> pair;
            while ((it != queue.end()) && (c < n)) {
                pair = process_agent(v, g);
                if(!((*it)->has_traveled()) && pair.second){
                    Vertex old_vertex = (*it)->get_vertex();
                    Vertex new_vertex = pair.first;
                    
                    (*it)->set_traveled(true);
                    (*it)->set_edge(new_vertex);
                    boost::get(&VertexProperty::queue, g, new_vertex).push_back(std::move((*it)));
                    it = queue.erase(it);
                    c++;
					s.update_flux();
                } else {
                    it++;
                }
            }
    }

    std::string get_perc_color(int i, int MAX) {
        // Ensure i is within the range [0, MAX]
        std::stringstream ss;
        if (i >= MAX) {
            ss << "#" << std::setfill('0') << std::setw(2) << std::hex << 255
                << std::setfill('0') << std::setw(2) << std::hex << 0
                << std::setfill('0') << std::setw(2) << std::hex << 0;
        }
        else {
            ss << "#" << std::setfill('0') << std::setw(2) << std::hex << 0
                << std::setfill('0') << std::setw(2) << std::hex << 0
                << std::setfill('0') << std::setw(2) << std::hex << 255;
        }
        return ss.str();
    }


    void print_mean_congestion(Graph& dual, int t, int N_AGENTS, double perc) {
        int congested_cntr = 0;
		std::ofstream file_perc("fig/graph_percolation_N"+std::to_string(N_AGENTS)+"_p"+std::to_string(int(std::round(perc*100))) + ".txt");

        //file for detection:
        for (int i = 0; i < boost::num_vertices(dual); i++) {
            auto it = std::find_if(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v) {return dual[v].index == i; });
            if (it == boost::vertices(dual).second) { throw std::runtime_error{ "Error" }; };
            if (double(dual[*it].congestion_time) / double(TIME_MAX_SIMULATION) >= perc) {
                congested_cntr++;
                dual[*it].occupied = true;
            }
            //file_comparation << i << " " << double(dual[*it].congestion_time) / double(TIME_MAX_SIMULATION) << std::endl;
        }




        auto vertex_writer_dual2 = [&](std::ostream& out, const auto& v) {
            out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", occupied = \"" << dual[v].occupied << "\" ]" ;
        };

        //boost::write_graphviz(file_dual, dual, vertex_writer_dual, edge_writer_dual);
        //file_dual.close();
        //std::system("fdp -Tpng fig/graph_mean_congestion.dot -o fig/graph_mean_congestion.png");
        boost::write_graphviz(file_perc, dual, vertex_writer_dual2, boost::default_writer());
        //file_dual.close();
        file_perc.close();
        
        
    }
    

    void print_info(Vertex v, Graph&  g_dual) {
        std::list<std::shared_ptr<Agent>>& queue = get(&VertexProperty::queue, g_dual, v);
        if (queue.size() != 0)
        {
            std::cout << "v. " << std::to_string(g_dual[v].index) << " | agents id : ";
            std::for_each(queue.begin(), queue.end(), [&](std::shared_ptr<Agent> const& a_ptr)
                { std::cout << a_ptr->get_id() << ", "; });
            std::cout << std::endl << std::endl;
        }
    }
}