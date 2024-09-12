#ifndef RENDER
#define RENDER

#include "./alias.hpp"
#include "./agents.hpp"
#include <SFML/Graphics.hpp>

#define M_PI 3.14159

namespace boost
{

    sf::Color interpolateColor(int x, int MAX_AGENTS)
    {
        // Clamp x tra 0 e 50
        if (x == 0){
            return sf::Color::Black;
        }
        if (x == MAX_AGENTS) {
            return sf::Color::Red;
        }
        if (x > MAX_AGENTS) {
            return sf::Color::Red;
        }

        // Calcola le componenti RGB del colore interpolato
        int r = static_cast<int>(255 * x / MAX_AGENTS);        // Interpolazione lineare del rosso
        int g = 0;                                     // Il verde è sempre 0
        int b = static_cast<int>(255 * (MAX_AGENTS - x) / MAX_AGENTS); // Interpolazione lineare del blu

        return sf::Color(r, g, b);
    }
    
    template<class G>
    void render_graph(sf::RenderWindow &w, Graph const &g, G const &dual, std::map<Vertex, Edge> dual_map, int SIDE, bool enable_dynamics)
    {
        auto id_map = get(&VertexProperty::index, g);

        double R = 10;
        int display_height = w.getSize().x;
        double L = (display_height) / (SIDE);

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

            sf::Vector2f starting_point = sf::Vector2f(0.015 * display_height + (0.5 + column_s) * L, 0.015 * display_height + L * (row_s + 0.5));
            sf::Vector2f target_point = sf::Vector2f(0.015 * display_height + (0.5 + column_t) * L, 0.015 * display_height + L * (row_t + 0.5));

            float distance = std::sqrt(std::pow(target_point.x - starting_point.x, 2) + std::pow(target_point.y - starting_point.y, 2));

            // Calcola l'angolo di rotazione
            float angle = std::atan2(target_point.y - starting_point.y, target_point.x - starting_point.x) * 180 / M_PI;

            // Crea il rettangolo
            sf::RectangleShape rectangle(sf::Vector2f(distance, 5.f)); // 5.f è la larghezza del rettangolo (abbastanza stretto)

            // Imposta la posizione del rettangolo al punto iniziale
            rectangle.setPosition(starting_point);

            // Ruota il rettangolo per allinearlo alla linea tra i due punti
            rectangle.setRotation(angle);
            int occ = boost::get(&VertexProperty::queue, dual, *v).size();
            rectangle.setFillColor(interpolateColor(occ,MAX_CAP));

            w.draw(rectangle);
        }

        
    }

};

#endif