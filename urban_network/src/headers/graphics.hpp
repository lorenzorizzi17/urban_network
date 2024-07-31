#ifndef GRAPHICS
#define GPAPHICS

#include<SFML/Graphics.hpp>
#include"stall.hpp"
#include"graph_builder.hpp"

namespace od {
    void set_stats_text(int display_height, int t, Graph const& dual, std::unique_ptr<sf::RenderWindow>& stats_window) {
        sf::Font font;
        if (!font.loadFromFile("fig/font.ttf")) {
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

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::P) {
                    print_dual_paint(dual);
                }
                break;
            default:
                break;
            }
        }
    }
}


#endif