#include<fstream>
#include"headers/statistics.hpp"
#include"headers/graphics.hpp"

//constructor (vector of indexes and number of nodes)
Statistics::Statistics(std::vector<int> index) {
    m_index = index;
	m_occ_matrix.resize(m_index.size());
}

//constructor (every node up to n-1 and number of nodes)
Statistics::Statistics(int n) {
    for (int i = 0; i < n; i++) {
        m_index.push_back(i);
    } 
    m_occ_matrix.resize(m_index.size());
}

int Statistics::get_flux() const {
    return m_inst_flux;
}

std::vector<int> Statistics::get_lifespan() const
{
    return m_lifespan;
}

std::vector<int> Statistics::get_occ(int ix) const {
    return m_occ_matrix[ix];
}

void Statistics::update_flux() {
    m_inst_flux++;
}

void Statistics::clear() {
    m_inst_flux = 0;
}

void Statistics::update(Graph& g) {
    //update the flux
    m_flux.push_back(m_inst_flux);
    //update the occupation matrix
    for (int i = 0; i < m_index.size(); i++) {
        auto it = std::find_if(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {return g[v].index == m_index[i]; });
        m_occ_matrix[i].push_back(g[*it].queue.size());
    }
}

void Statistics::update_lifespan(int a) {
    m_lifespan.push_back(a + 1);
}

void Statistics::save(Graph& m_dual, int time_max, int N) {
    //save occupation vs time
    for (int i = 0; i < m_index.size(); i++) {
        std::ofstream file;
        file.open("data/occupation_vs_time/OD_T" + std::to_string(time_max) + "_N" + std::to_string(N) + "_ix" + std::to_string(m_index[i]) + ".txt");
        for (int j = 0; j < m_occ_matrix[i].size(); j++) {
            file << j << " " << m_occ_matrix[i][j] << std::endl;
        }
        file.close();
    }

    std::ofstream file_flux;
    file_flux.open("data/flux/OD_T" + std::to_string(time_max) + "_N" + std::to_string(N) + ".txt", std::ios::app);
    for (int t = 0; t < m_flux.size(); t++) {
        file_flux << t << " " << m_flux[t] << std::endl;
    }
    file_flux.close();

    std::ofstream file_lifespan;
    file_lifespan.open("data/lifespan/OD_T" + std::to_string(time_max) + "_N" + std::to_string(N) + ".txt", std::ios::app);
    for (int t : m_lifespan) {
        file_lifespan << t << std::endl;
    }
    file_lifespan.close();
}