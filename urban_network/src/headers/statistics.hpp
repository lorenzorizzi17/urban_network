#ifndef STATISTICS

#include<vector>
#include<map>
#include<memory>

#include"alias.hpp"
#include<SFML/Graphics.hpp>

typedef std::map<int, int> Histogram;

class Statistics {
    public: 
    //constructor (vector of indexes and number of nodes)
    Statistics(std::vector<int>, int);

	//constructor (every node up to n-1 and number of nodes)
    Statistics(int, int);

    //get the inst flux
    int get_flux() const;

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
    void save(Graph&);

    //display graphically the data
    void display_data(std::unique_ptr<sf::RenderWindow>&, std::unique_ptr<sf::RenderWindow>&, std::unique_ptr<sf::RenderWindow>&, int);
    
    private:
        void draw_real_time_graph(sf::RenderWindow& m_main_window, int, int, int );
        void draw_real_time_histo_flux(sf::RenderWindow& m_main_window, int, Histogram&);
        void draw_histo_post(sf::RenderWindow& m_main_window, Histogram&);
    
    private:
        std::vector<int> m_flux;
        std::vector<int> m_lifespan;
        std::vector<int> m_index;
        std::vector<std::vector<int>> m_occ_matrix;

        int m_N;
        int m_inst_flux = 0;

        //buffer items
        std::vector<sf::RectangleShape> m_buffer;
        Histogram m_histo_flux;
        Histogram m_histo_occ;
};


#endif