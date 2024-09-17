#ifndef GRAPHICS
#define GRAPHICS

#include<SFML/Graphics.hpp>

void inline set_stats_text(float display_height, int t, Graph const& dual, std::unique_ptr<sf::RenderWindow>& stats_window) {
    sf::Font font;
    if (!font.loadFromFile("graph/font.ttf")) {
        throw std::runtime_error("Font non caricato"); // Gestisce l'errore se il font non viene caricato
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(0.05f * display_height, 0.025f * display_height);
    std::string s = "t = " + std::to_string(t) + "\nN. agents: " + std::to_string(2000) + "\nFlow rate: " + std::to_string(FLOW_RATE);
    text.setString(s);
    stats_window->draw(text);
}

void inline handle_events(std::unique_ptr<sf::RenderWindow>& m_main_window) {
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

#endif