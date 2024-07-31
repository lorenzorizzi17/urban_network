#ifndef STATISTICS
#define STATISTICS

#include<vector>
#include<map>
#include<string>
#include<numeric>
#include<cmath>


double mean(std::vector<int> v) {
    return double(std::accumulate(v.begin(), v.end(), 0)) / double((v.size()));
}


double sd(std::vector<int> v )
{
    double mn = mean(v);

    double var = 0;
    for (int i = 0; i < v.size(); i++) {
        var = var + (v[i] - mn)*(v[i] - mn);
    }
    return std::sqrt(var/double(v.size()));

}



class Statistics {
private:
    std::vector<int> m_flux;
    std::vector<int> m_lifespan;
    std::vector<int> m_occ_matrix[10];

    std::vector<int> m_index;
    int m_N;

    int m_inst_flux = 0;
public: 
    Statistics(std::vector<int> index,int N) {
        m_index = index;
        m_N = N;
    }

    void update_flux() {
        m_inst_flux++;
    }

    void clear() {
        m_inst_flux = 0;
    }

    void update(Graph& g) {
        m_flux.push_back(m_inst_flux);
        for (int i = 0; i < m_index.size();i++) {
            auto it = std::find_if(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {return g[v].index == m_index[i]; });
            m_occ_matrix[i].push_back(g[*it].queue.size());
        }
    }

    void update_lifespan(int a) {
        m_lifespan.push_back(a+1);
    }

    void save(int run) {
        //occupation vs time
        std::ofstream file[10];
        for (int i = 0; i < 10; i++) {
            file[i].open("fig/occupation_vs_time/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(m_N) + "_ix" + std::to_string(m_index[i]) + "_run" + std::to_string(run) + ".txt");
            for (int j = 0; j < m_occ_matrix[i].size(); j++) {
                file[i] << j << " " << m_occ_matrix[i][j] << std::endl;
            }
            file[i].close();
        }

        std::ofstream file_flux;
        file_flux.open("fig/flux/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(m_N) + "_run" + std::to_string(run)+".txt");
        for (int t = 0; t < m_flux.size(); t++) {
            file_flux << t << " " << m_flux[t] << std::endl;
        }
        file_flux.close();

        std::cout << mean(m_flux) << " +/- " << sd(m_flux) << std::endl;

        std::ofstream file_lifespan;
        file_lifespan.open("fig/lifespan/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(m_N) + "_run" + std::to_string(run) + ".txt");
        for (int t : m_lifespan) {
            file_lifespan << t << std::endl;
        }
        file_lifespan.close();
    }
};


#endif