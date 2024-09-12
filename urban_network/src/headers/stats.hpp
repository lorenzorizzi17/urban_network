#ifndef STATISTICS
#define STATISTICS

#include<vector>
#include<map>
#include<string>
#include<numeric>
#include<cmath>

class Statistics {
private:
    std::vector<int> m_flux;
    std::vector<int> m_lifespan;
    std::vector<int> m_occ_matrix[360];

    std::vector<int> m_index;
    int m_N;
    int m_inst_flux = 0;

public: 
    Statistics(std::vector<int> index,int N) {
        m_index = index;
        m_N = N;
    }

    Statistics(int N, int n) {
        m_N = N;
        for (int i = 0; i < n; i++) {
            m_index.push_back(i);
        }
    }

	int get_flux() const {
		return m_inst_flux;
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

    void save(Graph& m_dual) {
       
        for (int i = 0; i < m_index.size(); i++) {
            std::ofstream file;
            file.open("data/occupation_vs_time/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(m_N) + "_ix" + std::to_string(m_index[i]) + ".txt");
            for (int j = 0; j < m_occ_matrix[i].size(); j++) {
                file << j << " " << m_occ_matrix[i][j] << std::endl;
            }
            file.close();
        }

        std::ofstream file_flux;
        file_flux.open("data/flux/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(m_N) + ".txt", std::ios::app);
        for (int t = 0; t < m_flux.size(); t++) {
            file_flux << t << " " << m_flux[t] << std::endl;
        }
        file_flux.close();

        std::ofstream file_lifespan;
        file_lifespan.open("data/lifespan/OD_T" + std::to_string(TIME_MAX_SIMULATION) + "_N" + std::to_string(m_N) + ".txt", std::ios::app);
        for (int t : m_lifespan) {
            file_lifespan << t << std::endl;
        }
        file_lifespan.close();
    }
};


#endif