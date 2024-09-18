#include <SFML/Graphics.hpp>

#include "headers/alias.hpp"
#include "headers/agents.hpp"
#include "headers/ODModel.hpp"
#include"headers/statistics.hpp"

#define M_PI 3.14159

sf::Color interpolateColor(int x, int MAX_AGENTS)
{
    if (x == 0){
        return sf::Color::Black;
    }
    if (x >= MAX_AGENTS) {
        return sf::Color::Red;
    }

    int r = static_cast<int>(255 * x / MAX_AGENTS);   
    int g = 0;                                     
    int b = static_cast<int>(255 * (MAX_AGENTS - x) / MAX_AGENTS);

    return sf::Color(r, g, b);
}
    
void ODModel::render_graph(sf::RenderWindow&w, Graph const &g, Graph const &dual, std::map<Vertex, Edge>& dual_map){
    int SIDE = std::sqrt(m_config.N_NODES);
    auto id_map = get(&VertexProperty::index, g);
    //int display_height = w.getSize().y;
	int window_height = w.getSize().y;
	int window_lenght = w.getSize().x / 2.0;
    //road lenght
	float road_lenght = 5.0f;

	int h_offset = 0.05*window_height;
	int v_offset = 0.08*window_height;

    //draw a vertical line that divides the window in two (for ref)
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(window_lenght, 0), sf::Color::Black),  // Punto iniziale (X, Y)
		sf::Vertex(sf::Vector2f(window_lenght, window_height), sf::Color::Black)   // Punto finale (X, Y)
	};  
	w.draw(line, 2, sf::Lines);

    //background box
	sf::RectangleShape boxBkg(sf::Vector2f(window_lenght, window_height));
	boxBkg.setFillColor(sf::Color(0, 0, 125, 25));
	boxBkg.setPosition(0, 0);
	w.draw(boxBkg);


    //text section
	sf::Text title;
	sf::Font font;
	font.loadFromFile("graph/font.ttf");
	title.setFont(font);
	title.setString("Running simulation...");
	title.setCharacterSize(26);
	title.setStyle(sf::Text::Bold);
	title.setFillColor(sf::Color::Black);
	title.setOrigin(title.getLocalBounds().width /2.0, title.getLocalBounds().height / 2.0);
	title.setPosition(0.5 * window_lenght, 0.5*v_offset);
	w.draw(title);

    //lenght of a single cell
    double L = (window_lenght-2*h_offset) / (SIDE-1);
    // now draws the edges iterating over the dual graph
    Iter_Vertex vid, vfd;
    std::tie(vid, vfd) = boost::vertices(dual);
    for (Iter_Vertex v = vid; v != vfd; v++)
    {
        Vertex vsource = boost::source(dual_map.at(*v), g);
        Vertex vtarget = boost::target(dual_map.at(*v), g);
        int row_s = id_map[vsource] / SIDE;
        int column_s = id_map[vsource] - row_s * (SIDE);
        int row_t = id_map[vtarget] / SIDE;
        int column_t = id_map[vtarget] - row_t * (SIDE);

        sf::Vector2f starting_point = sf::Vector2f(h_offset + (0 + column_s) * L, v_offset + L * (row_s + 0));
        sf::Vector2f target_point = sf::Vector2f(h_offset + (0 + column_t) * L, v_offset + L * (row_t + 0));

        float distance = std::sqrt(std::pow(target_point.x - starting_point.x, 2) + std::pow(target_point.y - starting_point.y, 2));

        // Calcola l'angolo di rotazione
        float angle = std::atan2(target_point.y - starting_point.y, target_point.x - starting_point.x) * 180 / M_PI;

        // Crea il rettangolo
        sf::RectangleShape rectangle(sf::Vector2f(distance, road_lenght));

        // Imposta la posizione del rettangolo al punto iniziale
        rectangle.setPosition(starting_point);

        // Ruota il rettangolo per allinearlo alla linea tra i due punti
        rectangle.setRotation(angle);
        int occ = boost::get(&VertexProperty::queue, dual, *v).size();
        rectangle.setFillColor(interpolateColor(occ, m_config.MAX_CAP));

        w.draw(rectangle);
    }
}

void Statistics::display_data(std::unique_ptr<sf::RenderWindow>& m_main_window, int m_time, Config const& c) {
    if (c.LOG_OCCUPATION_VS_TIME) {
        draw_real_time_graph(*m_main_window, m_time, 100, c.LOG_OCCUPATION_VS_TIME_NODE, c.MAX_CAP);
    }
    if (c.LOG_HISTO_FLOW) {
        draw_real_time_histo_flux(*m_main_window, 30, m_histo_flux);
    }
    if (c.LOG_HISTO_OCC) {
        draw_histo_post(*m_main_window, m_histo_occ);
    }
    if (c.LOG_HISTO_LIFESPAN) {
        draw_histo_lifespan(*m_main_window, m_histo_lifespan, 50, m_index_lifespan);
    }
}

//box1
void Statistics::draw_real_time_graph(sf::RenderWindow& m_main_window, int time, int interval, int ix, int N_MAX)
{
    int index_box = 0;
    int h_offset = 20;
    int v_offset = 20;
	int box5_height = 0.2 * m_main_window.getSize().y;
	
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
    double box_lenght = ((m_main_window.getSize().x / 2.0) - 3 * h_offset)/2.f;
	int box_height = (m_main_window.getSize().y - 4*v_offset - box5_height) / 2.f;
    int effective_time = time % interval;

	sf::RectangleShape boxBkg(sf::Vector2f(box_lenght, box_height));
	boxBkg.setFillColor(sf::Color(0, 0, 125,25));
	boxBkg.setPosition(m_main_window.getSize().x /2 + h_offset, v_offset);
	m_main_window.draw(boxBkg);

    if (effective_time == 0) {
        m_buffer.clear();
    }
    //int occ = scale*(dual[*std::find_if(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v) {return dual[v].index == ix; })].queue).size();
    int occ = scale * this->get_occ(0).back();

    //draw the ticks
    for (int i = 0; i < 20; i += 5) {
        sf::RectangleShape tick(sf::Vector2f(h_offset / 1.8, 3));
        tick.setFillColor(sf::Color::Black);
        tick.setOrigin(h_offset / 2.4, 3);
        tick.setPosition(m_main_window.getSize().x / 2  + h_offset, box_height- v_offset - i * scale);
        m_main_window.draw(tick);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 +h_offset, box_height + v_offset - i * scale), sf::Color(119,136,153)),  // Punto iniziale (X, Y)
            sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 +box_lenght - h_offset, box_height + v_offset - i * scale), sf::Color(119,136,153))   // Punto finale (X, Y)
        };
        m_main_window.draw(line, 2, sf::Lines);
    }

    //draw the rectangles
    int rectangle_base = box_lenght / interval;
    sf::RectangleShape rectangle(sf::Vector2f(rectangle_base, occ));
    rectangle.setOrigin(rectangle_base / 2.0, occ / 2.0);
	if (occ/scale > N_MAX) {
        rectangle.setFillColor(sf::Color::Red);
    }
    else {
        rectangle.setFillColor(sf::Color::Blue);
    }
    

    rectangle.setPosition(m_main_window.getSize().x / 2 + h_offset + (effective_time + 0.5) * rectangle_base, box_height - occ / 2.0 + v_offset);
    m_buffer.push_back(rectangle);
    for (sf::RectangleShape s : m_buffer) {
        m_main_window.draw(s);
    }

    //draw a line that follow the x axis
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 +h_offset + (effective_time + 0.5) * rectangle_base, box_height +v_offset), sf::Color(0,0,153)),  // Punto iniziale (X, Y)
        sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 +h_offset + (effective_time + 0.5) * rectangle_base, 0), sf::Color(0,0,200))   // Punto finale (X, Y)
    };
    m_main_window.draw(line, 2, sf::Lines);

    //draws the axis
    sf::RectangleShape x_axis(sf::Vector2f(box_lenght, 3));
    x_axis.setFillColor(sf::Color::Black);
    x_axis.setPosition(m_main_window.getSize().x / 2 + h_offset, box_height + v_offset);
    m_main_window.draw(x_axis);
    sf::RectangleShape y_axis(sf::Vector2f(3, box_height));
    y_axis.setOrigin(0, box_height);
    y_axis.setFillColor(sf::Color::Black);
    y_axis.setPosition(m_main_window.getSize().x / 2 + h_offset, box_height+ v_offset);
    m_main_window.draw(y_axis);
}

void Statistics::draw_real_time_histo_flux(sf::RenderWindow& m_main_window, int bin, std::map<int, int>& histo) {
    int index_box = 0;
    int h_offset = 20;
    int v_offset = 20;
    int box5_height = 0.2 * m_main_window.getSize().y;
    double box_lenght = ((m_main_window.getSize().x / 2.0) - 3 * h_offset) / 2.f;
    int box_height = (m_main_window.getSize().y - 4 * v_offset - box5_height) / 2.f;

    sf::RectangleShape boxBkg(sf::Vector2f(box_lenght, box_height));
    boxBkg.setFillColor(sf::Color(0, 0, 255, 25));
    boxBkg.setPosition(m_main_window.getSize().x / 2 + 2 * h_offset + box_lenght, v_offset);
    m_main_window.draw(boxBkg);

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
    int phi_min = 700;
    int phi_max = 900;
	if ((actual_flux > phi_min)&&(actual_flux < phi_max)){
        double bin_l = box_lenght / double(bin);
        double bin_occ = (phi_max - phi_min) / bin;
        int bin_index = (actual_flux - phi_min) / bin_occ;
        histo[bin_index] += 1;

        int integral = 0;
        for (std::pair<int, int> p : histo) {
            integral += p.second;
        }

        for (std::pair<int, int> p : histo) {
            double heigh_norm = double(p.second) / double(integral);
            double height_rect = heigh_norm * 1700;
            sf::RectangleShape rectangle(sf::Vector2f(bin_l, height_rect));
            rectangle.setFillColor(sf::Color::Blue);
            rectangle.setOrigin(bin_l / 2., height_rect / 2.);
            rectangle.setPosition(m_main_window.getSize().x / 2.f + 2 * h_offset + box_lenght + (p.first + 0.5) * bin_l, v_offset+box_height - height_rect / 2);
            m_main_window.draw(rectangle);
        }
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
    int index_box = 0;
    int h_offset = 20;
    int v_offset = 20;
    int box5_height = 0.2 * m_main_window.getSize().y;
    double box_lenght = ((m_main_window.getSize().x / 2.0) - 3 * h_offset) / 2.f;
    int box_height = (m_main_window.getSize().y - 4 * v_offset - box5_height) / 2.f;

    int actual_occ = this->get_occ(0).back();
    int phi_min = 0;
    int phi_max = 25;
    int interval = (phi_max - phi_min);
    double bin_lenght = box_lenght / (interval);
    int bin_index = (actual_occ - phi_min);
    histodist[bin_index] += 1;

	sf::RectangleShape boxBkg(sf::Vector2f(box_lenght, box_height));
	boxBkg.setFillColor(sf::Color(0, 0, 255, 25));
	boxBkg.setPosition(m_main_window.getSize().x / 2 + h_offset, box_height+3*v_offset+box5_height);
	m_main_window.draw(boxBkg);



    int integral = 0;
    for (std::pair<int, int> p : histodist) {
        integral += p.second;
    }


    for (int i = 0; i < 25; i++) {
        if (i % 5 == 0) {
            sf::Vertex line[] =
            {
               sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset + (0.5f + i) * bin_lenght, m_main_window.getSize().y - v_offset), sf::Color(0,0,153)),  // Punto iniziale (X, Y)
               sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset + (0.5f + i) * bin_lenght,m_main_window.getSize().y - v_offset - box_height), sf::Color(0,0,200))   // Punto finale (X, Y)
            };
            m_main_window.draw(line, 2, sf::Lines);
        } else {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset+(0.5f + i) * bin_lenght, m_main_window.getSize().y-v_offset), sf::Color(0,0,153,10)),  // Punto iniziale (X, Y)
                sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset+(0.5f + i) * bin_lenght,m_main_window.getSize().y-v_offset - box_height), sf::Color(0,0,200,10))   // Punto finale (X, Y)
            };
            m_main_window.draw(line, 2, sf::Lines);
        }

    }


    for (std::pair<int, int> p : histodist) {
        double heigh_norm = double(p.second) / double(integral);
        double height_rect = heigh_norm * 2000;
		if (height_rect > box_height) {
			height_rect = box_height;
		}
        sf::RectangleShape rectangle(sf::Vector2f(bin_lenght, height_rect));
        rectangle.setFillColor(sf::Color::Blue);
        rectangle.setOrigin(bin_lenght / 2., height_rect / 2.);
        rectangle.setPosition(m_main_window.getSize().x/2+ h_offset + (p.first + 0.5) * bin_lenght, m_main_window.getSize().y - height_rect / 2 - v_offset);
        m_main_window.draw(rectangle);
    }
}


void Statistics::draw_histo_lifespan(sf::RenderWindow& m_main_window, std::map<int, int>& histolifespan, int bin, int& old_index) {
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
    int index_box = 0;
    int h_offset = 20;
    int v_offset = 20;
    int box5_height = 0.2 * m_main_window.getSize().y;
    double box_lenght = ((m_main_window.getSize().x / 2.0) - 3 * h_offset) / 2.f;
    int box_height = (m_main_window.getSize().y - 4 * v_offset - box5_height) / 2.f;
	int bin_lenght = box_lenght / bin;
	//std::cout << old_index << " < " << this->get_lifespan().size() << std::endl;
    for (int i = old_index; i < this->get_lifespan().size(); i++) {
        int actual_lifespan = this->get_lifespan()[i];
		int bin_index = actual_lifespan;
		histolifespan[bin_index] += 1;
	} 
    old_index = this->get_lifespan().size();

    sf::RectangleShape boxBkg(sf::Vector2f(box_lenght, box_height));
    boxBkg.setFillColor(sf::Color(0, 0, 255, 25));
    boxBkg.setPosition(m_main_window.getSize().x / 2 + 2 * h_offset + box_lenght, box_height + 3 * v_offset + box5_height);
    m_main_window.draw(boxBkg);

    int integral = 0;
    for (std::pair<int, int> p : histolifespan) {
        integral += p.second;
    }

    /*for (int i = 0; i < 25; i++) {
        if (i % 5 == 0) {
            sf::Vertex line[] =
            {
               sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset + (0.5f + i) * bin_lenght, m_main_window.getSize().y - v_offset), sf::Color(0,0,153)),  // Punto iniziale (X, Y)
               sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset + (0.5f + i) * bin_lenght,m_main_window.getSize().y - v_offset - box_height), sf::Color(0,0,200))   // Punto finale (X, Y)
            };
            m_main_window.draw(line, 2, sf::Lines);
        }
        else {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset + (0.5f + i) * bin_lenght, m_main_window.getSize().y - v_offset), sf::Color(0,0,153,10)),  // Punto iniziale (X, Y)
                sf::Vertex(sf::Vector2f(m_main_window.getSize().x / 2 + h_offset + (0.5f + i) * bin_lenght,m_main_window.getSize().y - v_offset - box_height), sf::Color(0,0,200,10))   // Punto finale (X, Y)
            };
            m_main_window.draw(line, 2, sf::Lines);
        }

    } */


    for (std::pair<int, int> p : histolifespan) {
        double heigh_norm = double(p.second) / double(integral);
        double height_rect = heigh_norm * 3000;
        if (height_rect > box_height) {
            height_rect = box_height;
        }
        if (p.first < 50) {
            sf::RectangleShape rectangle(sf::Vector2f(bin_lenght, height_rect));
            rectangle.setFillColor(sf::Color::Blue);
            rectangle.setOrigin(bin_lenght / 2., height_rect / 2.);
            rectangle.setPosition(m_main_window.getSize().x / 2 + h_offset * 2 + box_lenght + (p.first + 0.5) * bin_lenght, m_main_window.getSize().y - height_rect / 2 - v_offset);
            m_main_window.draw(rectangle);
        }
    }
}


void ODModel::render_stat_panel(sf::RenderWindow& m_main_window, Config const& c, int cycle_per_second) {
    int index_box = 0;
    int h_offset = 20;
    int v_offset = 20;
    int box5_height = 0.2 * m_main_window.getSize().y;
    double box_lenght = ((m_main_window.getSize().x / 2.0) - 3 * h_offset) / 2.f;
    int box_height = (m_main_window.getSize().y - 4 * v_offset - box5_height) / 2.f;
	double box_lenght5 = m_main_window.getSize().x / 2.0 - 2 * h_offset;

    //background boz
	sf::RectangleShape boxBkg(sf::Vector2f(box_lenght5, box5_height));
	boxBkg.setFillColor(sf::Color(0, 0, 125, 25));
	boxBkg.setPosition(m_main_window.getSize().x / 2.0f + h_offset, box_height + 2*v_offset);

	//text section
    sf::Font font;
    if (!font.loadFromFile("graph/font.ttf")) {
        throw std::runtime_error("Font non caricato"); // Gestisce l'errore se il font non viene caricato
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
    //title.setPosition(0.5 * window_lenght, 0.5 * v_offset);
    text.setPosition(m_main_window.getSize().x / 2.0f + h_offset + 0.01 * box_lenght5, box_height + 2 * v_offset + 0.5*box5_height);
	std::string s = "Running at " + std::to_string(cycle_per_second) + " cycle per second"; text.setString(s);
    m_main_window.draw(text);
	m_main_window.draw(boxBkg);
}
