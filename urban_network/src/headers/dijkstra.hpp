
#ifndef DIJKSTRA
#define DIJKSTRA

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "./alias.hpp"



namespace boost
{

    // compute the dijkstra shortest path between two vertices in the graph. Returns a vector containing the ordered sequence of vertices to get from A to B
    std::pair<std::vector<Edge>, double> get_dijkstra_shortest_path(Edge const &ei, Edge const &ef, Graph const &g, Graph const &g_dual, std::map<Edge, Vertex> &map)
    {
        Vertex vi = map[ei]; // corresponding road on dual graph
        Vertex vf = map[ef]; // corresponding road on dual graph
        boost::property_map<Graph, boost::vertex_index_t>::type map_indices = boost::get(boost::vertex_index, g_dual);
        // This is to store the predecessor
        std::vector<Vertex> p(num_vertices(g_dual));
        // This is to store the distances
        std::vector<double> d(num_vertices(g_dual));
        boost::dijkstra_shortest_paths(g_dual, vi, predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g_dual))).distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g_dual))));

        int index_f = map_indices[vf];
        Vertex pred = vf;
        std::vector<Edge> res;
        for (const auto &pair : map)
        {
            if (pair.second == vf)
            {
                res.push_back(pair.first);
                break; // Esci dal loop dopo aver trovato la chiave
            }
        }
        while (pred != vi)
        {
            for (const auto &pair : map)
            {
                if (pair.second == pred)
                {
                    res.push_back(pair.first);
                    break;
                }
            }
            pred = p[map_indices[pred]];
        }
        std::reverse(res.begin(), res.end());
        return std::make_pair(res, d[vf]);
    }

    // compute the dijkstra shortest path between two vertices in the graph. Returns a vector containing the ordered sequence of vertices to get from A to B
    std::pair<std::vector<Vertex>, double> get_dijkstra_shortest_path(Vertex const &vi, Vertex const &vf, Graph const &g)
    {
        boost::property_map<Graph, boost::vertex_index_t>::type map_indices = boost::get(boost::vertex_index, g);
        // This is to store the predecessor
        std::vector<Vertex> p(num_vertices(g));
        // This is to store the distances
        std::vector<double> d(num_vertices(g));
        boost::dijkstra_shortest_paths(g, vi, predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

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
        return std::make_pair(res, d[vf]);
    }

    std::tuple<Vertex, std::vector<Vertex>, double> get_vertex_based_on_dijkstra_shortest_path(Vertex const vi, Graph const &g)
    {
        boost::property_map<Graph, boost::vertex_index_t>::type map_indices = boost::get(boost::vertex_index, g);

        // This is to store the predecessor
        std::vector<Vertex> p(boost::num_vertices(g));
        // This is to store the distances
        std::vector<double> d(boost::num_vertices(g));
        // run dijkstra departing from vi. The algorithm will write in d the distances from vi to any other nodes (NAMED PARAMETERS BGL)
        boost::dijkstra_shortest_paths(g, vi,
                                       predecessor_map(boost::make_iterator_property_map(p.begin(), boost::get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(d.begin(), boost::get(boost::vertex_index, g))));
    
        // selecting a random arrival node whose distance is at least MIN_DIST_DIJKSTRA
        std::size_t num_vertices = boost::num_vertices(g);
        int i = std::rand() % num_vertices;
        double distance = d[i];
        while (distance < MIN_DIST_DIJKSTRA)
        {
            i = std::rand() % boost::num_vertices(g);
            distance = d[i];
        }
        Iter_Vertex ivf = boost::vertices(g).first;
        std::advance(ivf, i);
        Vertex vf = *ivf;

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

        return std::make_tuple(vf, res, distance);
    };
}

#endif
