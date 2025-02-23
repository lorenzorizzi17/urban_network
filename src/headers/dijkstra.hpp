#ifndef DIJKSTRA
#define DIJKSTRA

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "alias.hpp"
#include "config.hpp"



//call just once to create the static chart
inline Chart create_static_chart(Graph const& g)
{
	boost::property_map<Graph, boost::vertex_index_t>::type map_indices = boost::get(boost::vertex_index, g);
    Chart chart;
	for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++)
    {
		Vertex v = *it;
        // This is to store the predecessor
        std::vector<Vertex> p(boost::num_vertices(g));
        // This is to store the distances
        std::vector<double> d(boost::num_vertices(g));

        // run dijkstra departing from vi. The algorithm will write in d the distances from vi to any other nodes (NAMED PARAMETERS BGL)
        boost::dijkstra_shortest_paths(g, v, predecessor_map(boost::make_iterator_property_map(p.begin(), boost::get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(d.begin(), boost::get(boost::vertex_index, g))));

		chart.push_back(d);
	}
    return chart;
}
	

inline std::tuple<Vertex, std::vector<Vertex>, double> get_vertex_based_on_dijkstra_shortest_path(Vertex const vi, Graph const &g, Config const& c, Chart const& chart)
{
    boost::property_map<Graph, boost::vertex_index_t>::type map_indices = boost::get(boost::vertex_index, g);
	int index = map_indices[vi];
    //first, find the dest vertex
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, boost::num_vertices(g) - 1);
    int i = dis(gen);
    double distance = chart[index][i];
    Iter_Vertex vf = boost::vertices(g).first;
    std::advance(vf, i);
    int occ = get(&VertexProperty::queue, g, *vf).size();

    while ((distance < c.MIN_DIST_DIJKSTRA) || (occ >= c.MAX_SPAWNABLE)) {
        i = dis(gen);
        distance = chart[index][i];
        vf = boost::vertices(g).first;
        std::advance(vf, i);
        occ = get(&VertexProperty::queue, g, *vf).size();
    }

    // now vf is chosen. Run dijkstra
    // This is to store the predecessor
    std::vector<Vertex> p(boost::num_vertices(g));
    // This is to store the distances
    std::vector<double> d(boost::num_vertices(g));

    // run dijkstra departing from vi. The algorithm will write in d the distances from vi to any other nodes (NAMED PARAMETERS BGL)
    boost::dijkstra_shortest_paths(g, vi, predecessor_map(boost::make_iterator_property_map(p.begin(), boost::get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(d.begin(), boost::get(boost::vertex_index, g))));
        
    int index_f = map_indices[*vf];
    Vertex pred = *vf;
    std::vector<Vertex> res;
    res.push_back(pred);
    while (pred != vi)
    {
        pred = p[map_indices[pred]];
        res.push_back(pred);
    }
    std::reverse(res.begin(), res.end());

    return std::make_tuple(*vf, res, distance);
};


#endif