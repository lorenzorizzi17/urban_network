#ifndef STALL
#define STALL

#include<fstream>
#include<algorithm>

#include"alias.hpp"
#include <boost/graph/graphviz.hpp>


namespace od {

    void get_stall_graph(Graph& dual) {
        Graph g = dual;
        std::ofstream file("fig/stall2.dot");

        auto edge_writer = [&](std::ostream& out, Edge e) {
            Vertex v = boost::target(e, dual);
            if (!g[v].congested) {
            }
            else {
                out << "[color = red, penwidth = 3.0 ]";
            }
        };


        auto vertex_writer = [&](std::ostream& out, const Vertex v) {
            if (dual[v].congested == true) {
                out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"red\", style = \"filled\" ]";;
            }
            else {
                out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"blue\", style = \"filled\" ]";;
            }
            };


        boost::write_graphviz(file, dual, vertex_writer, edge_writer);
        file.close();

        std::system("fdp -Tpng fig/stall2.dot -o fig/stall2.png");
    }

    

    void build_stall_graph(Graph& dual) {
        Graph g = dual;
        std::ofstream file("fig/stall.dot");

        auto edge_writer = [&](std::ostream& out, Edge e) {
            Vertex v = boost::source(e, dual);
            std::list<std::shared_ptr<Agent>>& queue = get(&VertexProperty::queue, dual, v);
            auto it = std::find_if(queue.begin(), queue.end(), [&](std::shared_ptr<Agent>& a_ptr){ return a_ptr->get_next_vertex() == boost::target(e, dual);});
            if (it == queue.end()) {
            }
            else {
                out << "[color = red, penwidth = 3.0 ]";
            }
         };


        auto vertex_writer = [&](std::ostream& out, const Vertex v) {
            if (dual[v].full == true) {
                out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"red\", style = \"filled\" ]";;
            }
            else if (dual[v].queue.empty()) {
                out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"blue\", style = \"filled\" ]";
            }
            else {
                out << "[label=\"" << boost::get(&VertexProperty::index, dual, v) << "\", fillcolor = \"orange\", style = \"filled\" ]";
            }
        };


        boost::write_graphviz(file, dual, vertex_writer, edge_writer);
        file.close();

        std::system("fdp -Tpng fig/stall.dot -o fig/stall.png");
    }


    void d_check_for_partial_gridlock_b(Graph& g, int time) {
        int c = 0;
        std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {if (g[v].congested == true) { c++; }});
        if (c != 0) {
            throw partial_gridlock_exception{ "A node has encountered gridlock!", time,c };
        }
   
    }



    void d_check_for_gridlock_b(Graph& g, int time) {
        std::vector<int> v;
        for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
            std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, g, *it);
            if (!queue.empty()) {
                std::for_each(queue.begin(), queue.end(), [&](std::shared_ptr<Agent> const& a_ptr)
                    { v.push_back(g[a_ptr->get_next_vertex()].full); });
            }
            
        }

        auto it = std::find(v.begin(), v.end(), 0);
        if (it == v.end()) {
            int c = 0;
            std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {if (!g[v].queue.empty()) { c++; }});
            throw gridlock_exception{ "System has collapsed to gridlock!", time,c };
        }


    }

}
#endif