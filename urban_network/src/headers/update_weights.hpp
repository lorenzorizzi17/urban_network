#ifndef UPDATE_WEIGHTS
#define UPDATE_WEIGHTS

#include"alias.hpp"

void update_weights(Graph& dual) {

    auto range = boost::edges(dual);

    for (auto it_e = range.first; it_e != range.second; it_e++) {
        int occupancy_src = (dual[boost::source(*it_e, dual)].queue).size();
        int occupancy_tgt = (dual[boost::target(*it_e, dual)].queue).size();
        get(boost::edge_weight, dual, *it_e) = get(&EdgeProperty::initial_weight, dual, *it_e) + (occupancy_src + occupancy_tgt) * LINEAR_FACTOR;
    }
    /* auto itv = boost::vertices(dual);
    for (auto itvv = itv.first; itvv != itv.second; itvv++) {
        int occupancy = (dual[*itvv].queue).size();


        auto in_edges = boost::in_edges(*itvv, dual);
        for (auto it = in_edges.first; it != in_edges.second; ++it) {
            boost::put(boost::edge_weight, dual, *it, get(&EdgeProperty::initial_weight, dual, *it) + occupancy * LINEAR_FACTOR);
            //DEBUG("processing edge (" + std::to_string(dual[boost::source(*it, dual)].index) + " - " + std::to_string(dual[boost::target(*it, dual)].index)+"Added " + std::to_string(occupancy * LINEAR_FACTOR)+ " now " + std::to_string(get(boost::edge_weight, dual, *it)));
        }

        auto out_edges = boost::out_edges(*itvv, dual);
        for (auto it = out_edges.first; it != out_edges.second; ++it) {
            boost::put(boost::edge_weight, dual, *it, get(&EdgeProperty::initial_weight, dual, *it) + occupancy * LINEAR_FACTOR); ;
        }

        for (auto it = in_edges.first; it != in_edges.second; ++it) {
            //boost::put(boost::edge_weight, dual, *it, get(&EdgeProperty::initial_weight, dual, *it) + occupancy * LINEAR_FACTOR);
            DEBUG("visualizing edge (" + std::to_string(dual[boost::source(*it, dual)].index) + " - " + std::to_string(dual[boost::target(*it, dual)].index) + "Added " + std::to_string(occupancy * LINEAR_FACTOR) + " now " + std::to_string(get(boost::edge_weight, dual, *it)));
        }
    } */
    
}


void set_weights(Graph& dual) {
    //iterate on edges
    for (auto it = boost::edges(dual).first; it != boost::edges(dual).second; it++) {
        get(boost::edge_weight, dual, *it) = get(&EdgeProperty::initial_weight, dual, *it);
    }
}

void set_flag_vertices(Graph& g) {
    for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
        get(&VertexProperty::full, g, *it) = (get(&VertexProperty::queue, g, *it).size() == MAX_CAP);
    }
}

#endif