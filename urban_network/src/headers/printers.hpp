#ifndef PRINTERS
#define PRINTERS

#include<fstream>
#include <string>

#include"alias.hpp"
#include <boost/graph/graphviz.hpp>

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

/*
void print_mean_occupation(Graph& g) {
    std::string spec = "fig/mean_occupation/" + std::string(PROJECTNAME) + "_T" + std::to_string(TIME_MAX_SIMULATION) +"_N"+ std::to_string(N_AGENTS) + ".txt";
    std::ofstream file_comparation(spec);
    for (int i = 0; i < boost::num_vertices(g); i++) {
        auto it = std::find_if(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {return g[v].index == i; });
        if (it == boost::vertices(g).second) { throw std::runtime_error{ "Error" }; };
        file_comparation << i << " " << double(g[*it].congestion_time) / double(TIME_MAX_SIMULATION) << std::endl;
    }
}

void print_percolation_graph(Graph const& dual, int t) {
    std::string spec = "fig/perc_graph/" + std::string(PROJECTNAME) + "_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(N_AGENTS)+ "_th"+std::to_string(PERCOLATION_THRESHOLD);
    std::ofstream file_dual(spec+".dot");
    auto edge_writer_dual = [&](std::ostream& out, const Edge e) {
        out << "[weight=\"" << boost::get(boost::edge_weight, dual, e) << "\"]";
        };

    auto vertex_writer_dual = [&](std::ostream& out, const auto& v) {
        out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"" << get_perc_color(get(&VertexProperty::congestion_time, dual, v)/TIME_MAX_SIMULATION, double(PERCOLATION_THRESHOLD)) << "\", style = \"filled\" ]";
        };

    boost::write_graphviz(file_dual, dual, vertex_writer_dual, edge_writer_dual);
    file_dual.close();
    std::string a = "fdp -Tpng " + spec + ".dot -o " + spec + ".png";
    std::system(a.c_str());
}
*/

void build_parser(Parser const& p, int N) {
    std::ofstream file("fig/parser/parser_N"+ std::to_string(N)+".dot");
    auto edge_writer = [&](std::ostream& out, const Edge e) {
        PVertex v = boost::source(e, p);
        double prob = double(boost::get(boost::edge_weight, p, e)) / double(p[v].pass);
        out << "[probability=\"" << prob << "\"]";
        };


    auto vertex_writer = [&](std::ostream& out, const auto& v) {
        out << "[label=\"" << boost::get(&ParserProperty::index, p, v) << "\"]";
        };


    boost::write_graphviz(file, p, vertex_writer, edge_writer);
    file.close();
    //std::system("fdp -Tpng fig/parser.dot -o fig/parser.png");
}

void update_queue_size(std::ofstream& file, Graph& g, int time, int ix) {
    auto it = std::find_if(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {return g[v].index == ix; });
    if (it == boost::vertices(g).second) { throw std::runtime_error{ "Error" }; };
    double partial_mean = double(g[*it].congestion_time) / double(time);
    file << time << " " << partial_mean;

    file << std::endl;
}

#endif