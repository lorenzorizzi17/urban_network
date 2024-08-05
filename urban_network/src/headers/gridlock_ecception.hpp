#include <iostream>
#include <exception>
#include <string>
#include "alias.hpp"

// Classe di eccezione personalizzata
class gridlock_exception : public std::exception {
private:
    std::string message;
    int time;
    int congested_nodes;

public:
    gridlock_exception(const std::string& msg, int code, int nodes)
        : message(msg), time(code), congested_nodes(nodes) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    // Metodi per ottenere le informazioni aggiuntive
    int getTime() const {
        return time;
    }

    int getCongestedNodes() const {
        return congested_nodes;
    }

    void process() {
        std::unique_ptr<sf::RenderWindow> gridlock_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(100, 100), "Collapsed to gridlock!");
        gridlock_window->setFramerateLimit(60);
        sf::Font font;
        if (!font.loadFromFile("fig/font.ttf")) {
            throw std::runtime_error("Font non caricato"); // Gestisce l'errore se il font non viene caricato
        }
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(0.05 * 50, 0.025 * 50);
        while (gridlock_window->isOpen()) {
            sf::Event event;
            while (gridlock_window->pollEvent(event))
            {
                switch (event.type) {
                case sf::Event::Closed:
                    gridlock_window->close();
                    break;
                default:
                  break;
                }
            }
            gridlock_window->clear(sf::Color::Transparent);
            //disegna
            std::string s = "The system has reached a total gridlock!";
            text.setString(s);
            gridlock_window->draw(text);
            gridlock_window->display();
        }
    }

};

class partial_gridlock_exception : public std::exception {
private:
    std::string message;
    int time;
    int congested_nodes;

public:
    partial_gridlock_exception(const std::string& msg, int code, int nodes)
        : message(msg), time(code), congested_nodes(nodes) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    // Metodi per ottenere le informazioni aggiuntive
    int getTime() const {
        return time;
    }

    int getCongestedNodes() const {
        return congested_nodes;
    }

};