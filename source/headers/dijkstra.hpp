#ifndef DIJKSTRA
#define DIJKSTRA


#include </home/lorenzo17/boost/boost/graph/dijkstra_shortest_paths.hpp>
#include "./alias.hpp"

// dijkstra visitor, if ever needed
template <typename PropertyEdgeVertex, typename PropertyMapVertex>
class djk_index_printer : public boost::default_dijkstra_visitor
{
private:
    PropertyEdgeVertex m_name_map;
    PropertyMapVertex m_index_map;

public:
    djk_index_printer(PropertyEdgeVertex weight_map, PropertyMapVertex index_map) : m_name_map(weight_map), m_index_map(index_map) {} // ctor

    void discover_vertex(Vertex u, const Graph &g) const
    {
        std::cout << get(m_index_map, u) << " ";
    }
};

namespace boost
{

    // compute the dijkstra shortest path between two vertices in the graph. Returns a vector containing the ordered sequence of vertices to get from A to B
    std::pair<std::vector<Vertex>,double> get_dijkstra_shortest_path(Vertex const &vi, Vertex const &vf, Graph const &g)
    {
        boost::property_map<Graph, boost::vertex_index_t>::type map_indices = boost::get(boost::vertex_index, g);
        // This is to store the predecessor
        std::vector<Vertex> p(num_vertices(g));
        // This is to store the distances
        std::vector<double> d(num_vertices(g));
        boost::dijkstra_shortest_paths(g, vi,
                                       predecessor_map(boost::make_iterator_property_map(
                                                           p.begin(), get(boost::vertex_index, g)))
                                           .distance_map(boost::make_iterator_property_map(
                                               d.begin(), get(boost::vertex_index, g))));
        int index_f = map_indices[vf];
        Vertex pred = vf;
        std::vector<Vertex> res;
        res.push_back(pred);
        while (pred != vi)
        {
            pred = p[map_indices[pred]];
            res.push_back(pred);
        }
        std::reverse(res.begin(), res.end());
        return std::make_pair(res,d[vf]);
    }
}

#endif