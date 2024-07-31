#ifndef UPDATE_WEIGHTS
#define UPDATE_WEIGHTS

#include"alias.hpp"
#include"agents.hpp"
#include"graph_builder.hpp"
#include <boost/graph/graphviz.hpp>

namespace od {
    double f(double x) {
        return 0.001f * (x - 1) * (x - 1) * (x - 1) * (x - 1);
    }

    void update_weights(Graph& dual) {
        double LINEAR_FACTOR = 2 * double(ROAD_WEIGHT) / double(MAX_CAP);
        auto range = boost::edges(dual);

        for (auto it_e = range.first; it_e != range.second; it_e++) {
            int occupancy_src = (dual[boost::source(*it_e, dual)].queue).size();
            int occupancy_tgt = (dual[boost::target(*it_e, dual)].queue).size();
            get(boost::edge_weight, dual, *it_e) = get(&EdgeProperty::initial_weight, dual, *it_e) + f(occupancy_src + occupancy_tgt);
        }
    }


    void set_weights(Graph& dual) {
        //iterate on edges
        for (auto it = boost::edges(dual).first; it != boost::edges(dual).second; it++) {
            get(boost::edge_weight, dual, *it) = get(&EdgeProperty::initial_weight, dual, *it);
        }
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


            bool count = true;
            std::list<std::shared_ptr<od::Agent>>& queue = get(&VertexProperty::queue, g, *it);
            if (!queue.empty()){
                for (auto it2 = queue.begin(); it2 != queue.end(); it2++) {
                    //std::cout << ((*it2)->get_perm_time() > 500) << " ";
                    if ((*it2)->get_perm_time() > 500) {
                        count = count && true;
                    }
                    else {
                        count = count && false;
                    }
                }
                get(&VertexProperty::congested, g, *it) = count;
            }
        }
    }


    

}


#endif