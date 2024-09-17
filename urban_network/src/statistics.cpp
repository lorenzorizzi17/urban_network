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


void Statistics::display_data(std::unique_ptr<sf::RenderWindow>& m_graph_window, std::unique_ptr<sf::RenderWindow>& m_histo_window, std::unique_ptr<sf::RenderWindow>& m_histo_post_window, int m_time, Config const& c) {
if(c.LOG_OCCUPATION_VS_TIME){
    m_graph_window->clear(sf::Color(230, 230, 250));
    draw_real_time_graph(*m_graph_window, m_time, 100, c.LOG_OCCUPATION_VS_TIME_NODE);
    m_graph_window->display();
}

if (c.LOG_HISTO_FLOW){
    m_histo_window->clear(sf::Color(230, 230, 250));
    draw_real_time_histo_flux(*m_histo_window, 30, m_histo_flux);
    m_histo_window->display();
}

if(c.LOG_HISTO_OCC){
    m_histo_post_window->clear(sf::Color(230, 230, 250));
    draw_histo_post(*m_histo_post_window, m_histo_occ);
    m_histo_post_window->display();
}
}


void Statistics::draw_real_time_graph(sf::RenderWindow& m_main_window, int time, int interval, int ix)
{
    int horizantal_offset = 10;
    int vertical_offset = 10;
    int scale = 15;
    sf::Event event;
    while (m_main_window.pollEvent(event))
    {
        switch (event.type) {
        case sf::Event::Closed:
            m_main_window.close();
            break;
        default:
            break;
        }
    }
    int lenght = (m_main_window.getSize().x) - 2 * horizantal_offset;
    int effective_time = time % interval;
    int heightWindow = m_main_window.getSize().y;


    if (effective_time == 0) {
        m_buffer.clear();
    }
    //int occ = scale*(dual[*std::find_if(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v) {return dual[v].index == ix; })].queue).size();
    int occ = scale * this->get_occ(0).back();

    

    //draw the ticks
    for (int i = 0; i < 20; i += 5) {
        sf::RectangleShape tick(sf::Vector2f(horizantal_offset / 1.2, 3));
        tick.setFillColor(sf::Color::Black);
        tick.setOrigin(horizantal_offset / 2.4, 3);
        tick.setPosition(horizantal_offset, heightWindow - vertical_offset - i * scale);
        m_main_window.draw(tick);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(horizantal_offset, heightWindow - vertical_offset - i * scale), sf::Color(119,136,153)),  // Punto iniziale (X, Y)
            sf::Vertex(sf::Vector2f(m_main_window.getSize().x - horizantal_offset, heightWindow - vertical_offset - i * scale), sf::Color(119,136,153))   // Punto finale (X, Y)
        };
        m_main_window.draw(line, 2, sf::Lines);
    }


    int rectangle_base = lenght / interval;
    sf::RectangleShape rectangle(sf::Vector2f(rectangle_base, occ));
    rectangle.setOrigin(rectangle_base / 2.0, occ / 2.0);
    rectangle.setFillColor(sf::Color::Blue);

    rectangle.setPosition(horizantal_offset + (effective_time + 0.5) * rectangle_base, heightWindow - occ / 2.0 - vertical_offset);
    m_buffer.push_back(rectangle);
    for (sf::RectangleShape s : m_buffer) {
        m_main_window.draw(s);
    }

    //draw a line that follow the x axis
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(horizantal_offset + (effective_time + 0.5) * rectangle_base, heightWindow - vertical_offset), sf::Color(0,0,153)),  // Punto iniziale (X, Y)
        sf::Vertex(sf::Vector2f(horizantal_offset + (effective_time + 0.5) * rectangle_base, 0), sf::Color(0,0,200))   // Punto finale (X, Y)
    };
    m_main_window.draw(line, 2, sf::Lines);

    //draws the axis
    sf::RectangleShape x_axis(sf::Vector2f(lenght + horizantal_offset, 3));
    x_axis.setFillColor(sf::Color::Black);
    x_axis.setPosition(0, heightWindow - vertical_offset);
    m_main_window.draw(x_axis);
    sf::RectangleShape y_axis(sf::Vector2f(3, heightWindow));
    y_axis.setOrigin(0, heightWindow);
    y_axis.setFillColor(sf::Color::Black);
    y_axis.setPosition(horizantal_offset, heightWindow - vertical_offset);
    m_main_window.draw(y_axis);
}

void Statistics::draw_real_time_histo_flux(sf::RenderWindow& m_main_window, int bin, std::map<int, int>& histo) {
    sf::Event event;
    while (m_main_window.pollEvent(event))
    {
        switch (event.type) {
        case sf::Event::Closed:
            m_main_window.close();
            break;
        default:
            break;
        }
    }
    int actual_flux = this->get_flux();

	std::cout << actual_flux << std::endl;
    int phi_min = 600;
    int phi_max = 800;
    double bin_lenght = (phi_max - phi_min) / bin;
    int interval = (phi_max - phi_min);
    int bin_index = (actual_flux - phi_min) / bin_lenght;
    histo[bin_index] += 1;

    int integral = 0;
    for (std::pair<int, int> p : histo) {
        integral += p.second;
    }

    for (std::pair<int, int> p : histo) {
        double heigh_norm = double(p.second) / double(integral);
        double height_rect = heigh_norm * 2000;
        sf::RectangleShape rectangle(sf::Vector2f(bin_lenght, height_rect));
        rectangle.setFillColor(sf::Color::Blue);
        rectangle.setOrigin(bin_lenght / 2., height_rect / 2.);
        rectangle.setPosition((p.first + 0.5) * bin_lenght, m_main_window.getSize().y - height_rect / 2);
        m_main_window.draw(rectangle);
    }
}


void Statistics::draw_histo_post(sf::RenderWindow& m_main_window, std::map<int, int>& histodist) {
    sf::Event event;
    while (m_main_window.pollEvent(event))
    {
        switch (event.type) {
        case sf::Event::Closed:
            m_main_window.close();
            break;
        default:
            break;
        }
    }
    int actual_occ = this->get_occ(0).back();
    int phi_min = 0;
    int phi_max = 25;
    int interval = (phi_max - phi_min);
    double bin_lenght = m_main_window.getSize().x / (interval);
    int bin_index = (actual_occ - phi_min);
    histodist[bin_index] += 1;



    int integral = 0;
    for (std::pair<int, int> p : histodist) {
        integral += p.second;
    }


	for (int i = 0; i < 25; i++) {
        if (i % 5 == 0) {
            sf::Vertex line[] =
            {
               sf::Vertex(sf::Vector2f((0.5f + i) * bin_lenght, m_main_window.getSize().y), sf::Color(0,0,153)),  // Punto iniziale (X, Y)
               sf::Vertex(sf::Vector2f((0.5f + i) * bin_lenght, 0), sf::Color(0,0,200))   // Punto finale (X, Y)
            };
            m_main_window.draw(line, 2, sf::Lines);
        }
        else {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f((0.5f + i) * bin_lenght, m_main_window.getSize().y), sf::Color(0,0,153,10)),  // Punto iniziale (X, Y)
                sf::Vertex(sf::Vector2f((0.5f + i) * bin_lenght, 0), sf::Color(0,0,200,10))   // Punto finale (X, Y)
            };
            m_main_window.draw(line, 2, sf::Lines);
        }
        
	}


    for (std::pair<int, int> p : histodist) {
        double heigh_norm = double(p.second) / double(integral);
        double height_rect = heigh_norm * 2000;
        sf::RectangleShape rectangle(sf::Vector2f(bin_lenght, height_rect));
        rectangle.setFillColor(sf::Color::Blue);
        rectangle.setOrigin(bin_lenght / 2., height_rect / 2.);
        rectangle.setPosition((p.first + 0.5) * bin_lenght, m_main_window.getSize().y - height_rect / 2);
        m_main_window.draw(rectangle);
    }
}
