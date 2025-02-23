#ifndef STATISTICS
#define STATISTICS

#include<vector>
#include<map>
#include<memory>

#include"alias.hpp"
#include"config.hpp"
#include<SFML/Graphics.hpp>

typedef std::map<int, int> Histogram;

class Statistics {
    public: 
    //constructor (vector of indexes and number of nodes)
    Statistics(std::vector<int>);

	//constructor (every node up to n-1 and number of nodes)
    Statistics(int);

    //get the inst flux
    int get_flux() const;

    //get the inst lifespan
	std::vector<int> get_lifespan() const;

	//get the occupation history of the node with index ix
    std::vector<int> get_occ(int) const;

    //when called, update the inst flux
    void update_flux();

	//when called, clear the inst flux
    void clear();

    //update the statistics
    void update(Graph&);

    //separetely update the lifespan stats
    void update_lifespan(int);

    //save the stats in .txt files
    void save(Graph&, int, int);

    //display graphically the data
    void display_data(std::unique_ptr<sf::RenderWindow>&, int, Config const&);
    
    private:
        void draw_real_time_graph(sf::RenderWindow& , int, int, int, int);
        void draw_agent_vs_time(sf::RenderWindow&, int, int, int, int);
        void draw_real_time_histo_flux(sf::RenderWindow& , int,int,int, Histogram&);
        void draw_histo_post(sf::RenderWindow& , Histogram&);
		void draw_histo_occ(sf::RenderWindow& , int, int, int, int, Histogram&); 
        void draw_histo_lifespan(sf::RenderWindow&, Histogram&, int, int&, int);
    private:
        std::vector<int> m_flux;
        std::vector<int> m_lifespan;
        std::vector<int> m_index;
        std::vector<std::vector<int>> m_occ_matrix;

        int m_inst_flux = 0;

        //buffer items
        std::vector<sf::RectangleShape> m_buffer;
		std::vector<sf::RectangleShape> m_buffer_agent;
        Histogram m_histo_flux;
        Histogram m_histo_occ;
		Histogram m_histo_lifespan;
        int m_index_lifespan = 0;

};


#endif