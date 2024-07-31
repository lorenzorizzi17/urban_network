#include<numeric>

#include"alias.hpp"
#include"agents.hpp"


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
                
            }
            else {
                get(&VertexProperty::full, g, *it) = false;
            }

            get(&VertexProperty::congestion_time, g, *it) += get(&VertexProperty::queue, g, *it).size();
        }
    }

    std::pair<Vertex, bool> process_agent(Vertex v, Graph& g) {
        double rn = double(std::rand()) / double(RAND_MAX);
        double cum_p = 0;
        for (auto it2 = boost::out_edges(v, g).first; it2 != boost::out_edges(v, g).second; it2++) {
            cum_p = cum_p + get(&EdgeProperty::prob, g, *it2);
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

        //A value of 1 has not been reached. Choose a random road
        std::cout << "Miss";
        int rr = std::rand() & (boost::out_degree(v, g)-1);
        auto ch = boost::out_edges(v, g).first;
        std::advance(ch, rr);
            Vertex v_tar = boost::target(*ch, g);
            if (get(&VertexProperty::full, g, v_tar)) {
                return std::make_pair(0, false);
            }
            else {
                return std::make_pair(boost::target(*ch, g), true);
            }


    }

    void flow(Vertex v, Graph& g, int n, int& flux) {

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
                    flux++;
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


    void print_mean_congestion(Graph const& dual, int t) {
        std::ofstream file_dual("fig/graph_mean_congestion.dot");
        auto edge_writer_dual = [&](std::ostream& out, const Edge e) {
            out << "[weight=\"" << boost::get(boost::edge_weight, dual, e) << "\"]";
            };

        auto vertex_writer_dual = [&](std::ostream& out, const auto& v) {
            out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"" << get_perc_color(get(&VertexProperty::congestion_time, dual, v), t * (double(PERCOLATION_THRESHOLD) / double(100))) << "\", style = \"filled\" ]";
            };

        boost::write_graphviz(file_dual, dual, vertex_writer_dual, edge_writer_dual);
        file_dual.close();
        std::system("fdp -Tpng fig/graph_mean_congestion.dot -o fig/graph_mean_congestion.png");

        //file for detection:
        std::ofstream file_comparation("fig/comp.txt");
        for (int i = 0; i < boost::num_vertices(dual); i++) {
            auto it = std::find_if(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v) {return dual[v].index == i; });
            if (it == boost::vertices(dual).second) { throw std::runtime_error{ "Error" }; };
            file_comparation << i << " " << double(dual[*it].congestion_time)/double(   TIME_MAX_SIMULATION) << std::endl;
        }
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