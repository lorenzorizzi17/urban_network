#ifndef FLOW
#define FLOW

#include"./alias.hpp"
#include"./agents.hpp"
#include<algorithm>
#include<vector>

namespace boost {

    void flow(Edge const& e, Graph const& g, int MAX_FLOW, std::vector<Agent>& agents ){
    for(int c = 0; c < MAX_FLOW;c++){
        auto road = std::find_if(agents.begin(),agents.end(), [&](Agent const& a){return (e==a.get_road());} );
        if(road == agents.end()){
            return;
        } else {
            road->evolve_dijsktra();
        }
    }
}
}

#endif