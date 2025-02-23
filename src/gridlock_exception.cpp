#include<SFML/Graphics.hpp>
#include"headers/gridlock_exception.hpp"
#include"headers/agents.hpp"


gridlock_exception::gridlock_exception(int code, int nodes) : time(code), congested_nodes(nodes) {}


int gridlock_exception::getTime() const {
    return time;
}

int gridlock_exception::getCongestedNodes() const {
    return congested_nodes;
}

void gridlock_exception::process() {
    std::unique_ptr<sf::RenderWindow> gridlock_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(450, 60), "Collapsed to gridlock!", sf::Style::None || sf::Style::Close);
    gridlock_window->setFramerateLimit(60);
    sf::Font font;
    if (!font.loadFromFile("graph/font.ttf")) {
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
        gridlock_window->clear(sf::Color::White);
        //disegna
        std::string s = "The system has reached a total gridlock!";
        text.setString(s);
        gridlock_window->draw(text);
        gridlock_window->display();
    }
};

void check_for_gridlock(Graph& g, int time) {
    if (Agent::m_existing_agent != 0) {
        std::vector<int> v;
        for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
            std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, g, *it);
            if (!queue.empty()) {
                std::for_each(queue.begin(), queue.end(), [&](std::shared_ptr<Agent>& a_ptr)
                    { v.push_back(g[a_ptr->get_next_vertex()].full); });
            }
        }

        auto it = std::find(v.begin(), v.end(), 0);
        if (it == v.end()) {
            int c = 0;
            std::for_each(boost::vertices(g).first, boost::vertices(g).second, [&](Vertex v) {if (!g[v].queue.empty()) { c++; }});
            throw gridlock_exception{ time, c };
        }
    }
}