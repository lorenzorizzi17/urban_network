#ifndef GRAPHICS
#define GPAPHICS

#include<SFML/Graphics.hpp>
#include"gridlock.hpp"
#include"graph_builder.hpp"

namespace od {

    void set_stats_text(int display_height, int t, Graph const& dual, std::unique_ptr<sf::RenderWindow>& stats_window) {
        sf::Font font;
        if (!font.loadFromFile("graph/font.ttf")) {
            throw std::runtime_error("Font non caricato"); // Gestisce l'errore se il font non viene caricato
        }
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(0.05 * display_height, 0.025 * display_height);
        std::string s = "t = " + std::to_string(t) + "\nN. agents: " + std::to_string(d_check_sum(dual)) + "\nFlow rate: " + std::to_string(FLOW_RATE);
        text.setString(s);
        stats_window->draw(text);
    }



    void handle_events(std::unique_ptr<sf::RenderWindow>& m_main_window, Graph& dual) {
        sf::Event event;
        while (m_main_window->pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                m_main_window->close();
                break;
            default:
                break;
            }
        }
    }


    void draw_real_time_graph(sf::RenderWindow& m_main_window, Graph& dual, int time, int interval, int ix, std::vector<sf::RectangleShape>& buffer)
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
		int lenght = (m_main_window.getSize().x)-2*horizantal_offset;
        int effective_time = time % interval;
        int heightWindow = m_main_window.getSize().y;


		if (effective_time == 0) {
            buffer.clear();
		}
		int occ = scale* (dual[*std::find_if(boost::vertices(dual).first, boost::vertices(dual).second, [&](Vertex v) {return dual[v].index == ix; })].queue).size();
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
        rectangle.setOrigin(rectangle_base/2.0,occ/ 2.0);
		rectangle.setFillColor(sf::Color::Blue);
		
		rectangle.setPosition(horizantal_offset+(effective_time+0.5)*rectangle_base, heightWindow - occ/2.0 - vertical_offset);
        buffer.push_back(rectangle);
        for (sf::RectangleShape s : buffer) {
            m_main_window.draw(s);
        }

        //draws the axis
		sf::RectangleShape x_axis(sf::Vector2f(lenght+horizantal_offset, 3));
		x_axis.setFillColor(sf::Color::Black);
		x_axis.setPosition(0, heightWindow - vertical_offset);
		m_main_window.draw(x_axis);
		sf::RectangleShape y_axis(sf::Vector2f(3, heightWindow));
        y_axis.setOrigin(0, heightWindow);
		y_axis.setFillColor(sf::Color::Black);
		y_axis.setPosition(horizantal_offset,heightWindow-vertical_offset );
		m_main_window.draw(y_axis);

        
    }
}


#endif