#ifndef GRAPHICS
#define GRAPHICS

#include<SFML/Graphics.hpp>
#include"config.hpp"

void inline handle_events(std::unique_ptr<sf::RenderWindow>& m_main_window, bool& is_running) {
    sf::Event event;
    while (m_main_window->pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::Closed:
                m_main_window->close();
                break;
            case sf::Event::KeyPressed:
			    switch (event.key.code) {
                    case sf::Keyboard::S:
						is_running = !is_running;
						break;
                    default:
                        break;
			    }
			    break;
            default:
                break;
        }
    }
}

#endif