#ifndef RENDER
#define RENDER

#include "./alias.hpp"
#include "./agents.hpp"
#include <SFML/Graphics.hpp>

#define M_PI 3.14159

namespace boost
{

    // std::map<int,sf::Color > map{{0,sf::Color::Black}, {2,sf::Color::Blue}, {1,sf::Color::Red},{3,sf::Color::Green},{4,sf::Color::Cyan}};

    sf::Color interpolateColor(int x, int MAX_AGENTS)
    {
        // Clamp x tra 0 e 50
        if (x == 0){
            return sf::Color::Black;
        }

        // Calcola le componenti RGB del colore interpolato
        int r = static_cast<int>(255 * x / MAX_AGENTS);        // Interpolazione lineare del rosso
        int g = 0;                                     // Il verde è sempre 0
        int b = static_cast<int>(255 * (MAX_AGENTS - x) / MAX_AGENTS); // Interpolazione lineare del blu

        return sf::Color(r, g, b);
    }

    void render_graph(sf::RenderWindow &w, Graph const &g, Graph const &dual, std::map<Vertex, Edge> dual_map, int SIDE, bool enable_dynamics)
    {
        auto id_map = boost::get(&VertexProperty::index, g);

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
            rectangle.setFillColor(interpolateColor(occ,MAX_CAP+1));

            w.draw(rectangle);
        }

        // draws the grid
        for (int cur_index = 0; cur_index != SIDE * SIDE; cur_index++)
        {
            int row = cur_index / SIDE;
            int column = cur_index - row * (SIDE);

            sf::CircleShape unity(R);
            unity.setPosition(0.015 * display_height + (0.5 + column) * L, 0.015 * display_height + L * (row + 0.5));
            unity.setOrigin(R / 2, R / 2);
            int occ = 0;
            unity.setFillColor(sf::Color::Black);
            w.draw(unity);
        }
    }

    // STILL WIP
    /*    void render_dual_graph(sf::RenderWindow& w, Graph& dual, std::map<Vertex,Edge> dual_map, int SIDE, bool enable_dynamics){
           Iter_Vertex vi, vf;
           std::tie(vi,vf) = boost::vertices(dual);
           double R = 5;
           int display_height = w.getSize().x;
           double L = 0.8;
           double l = L*display_height/(2*SIDE-2);
           //backbone
           for (int ix=1 ;ix <(2*SIDE-1)*(2*SIDE-1); ix+=2,ei++)
           {
               sf::CircleShape unity(R);

               int row = ix/((2*SIDE-1));
               int column = ix-row*((2*SIDE-1));

               unity.setPosition((0.5-0.5*L)*display_height + (column)*l , (0.5-0.5*L)*display_height + l*(row));
               unity.setOrigin(R/2,R/2);

               int occ = 0;
               std::for_each(agents.begin(),agents.end(),[&](Agent const& a){if(a.get_road()==*ei){occ++;}});
               unity.setFillColor(map.at(occ));
               w.draw(unity);
           }
           //horizonatal lines
           for (int i = 0; i < 2*(SIDE); i+=2)
           {
                   sf::VertexArray lines(sf::LinesStrip, 2);
                   sf::Vector2f starting_point = sf::Vector2f((0.5-0.5*L)*display_height +l  ,(0.5-0.5*L)*display_height + l*(i) );
                   sf::Vector2f target_point = sf::Vector2f((0.5-0.5*L)*display_height +l*(1+2*(SIDE-2)), (0.5-0.5*L)*display_height + l*(i));
                   lines[0].position = starting_point;
                   lines[1].position = target_point;
                   lines[0].color = sf::Color::Black;
                   lines[1].color = sf::Color::Black;
                   w.draw(lines);
                   //vertical lines
                   sf::VertexArray lines_v(sf::LinesStrip, 2);
                   sf::Vector2f starting_point_v = sf::Vector2f((0.5-0.5*L)*display_height +l*(i)  ,(0.5-0.5*L)*display_height + l );
                   sf::Vector2f target_point_v = sf::Vector2f((0.5-0.5*L)*display_height +l*(i), (0.5-0.5*L)*display_height + l*(2*(SIDE-2)+1));
                   lines_v[0].position = starting_point_v;
                   lines_v[1].position = target_point_v;
                   lines_v[0].color = sf::Color::Black;
                   lines_v[1].color = sf::Color::Black;
                   w.draw(lines_v);
           }

           for (int i = 0; i < 2*(SIDE)-1; i++)
           {

           }
       }
    */

};

#endif