#include"./alias.hpp"
#include"./agents.hpp"
#include<SFML/Graphics.hpp>


namespace boost{
    
    void render_graph(sf::RenderWindow& w, Graph const& g, int SIDE, std::vector<Agent> agents, bool enable_dynamics){
        IdMap id_map = boost::get(boost::vertex_index,g);
        Iter_Vertex vi, vf;
        std::tie(vi,vf) = boost::vertices(g);
        double R = 5;
        int display_height = w.getSize().x;
        double L = (display_height)/(2*SIDE);

        for (Iter_Vertex v = vi; v != vf; v++)
        {
            int cur_index = id_map[*v];
            int row = cur_index/SIDE;
            int column = cur_index-row*(SIDE);
            
            sf::CircleShape unity(R);
               
            unity.setPosition(0.25*display_height + (0.5+column)*L , 0.25*display_height + L*(row+0.5));
            unity.setOrigin(R/2,R/2);
            int occ = 0;
            unity.setFillColor(sf::Color::Black);
            /* if(true){
                std::for_each(agents.begin(),agents.end(),[&](Agent const& a){if(a.get_vertex()==*v){occ++;}});
            switch (occ)
            {
            case 0:
                unity.setFillColor(sf::Color::Black);
                break;
            case 1:
                unity.setFillColor(sf::Color::Yellow);
                break;
            case 2:
                unity.setFillColor(sf::Color::Red);
                break;
            case 3:
                unity.setFillColor(sf::Color::Blue);
                break;
            default:
                unity.setFillColor(sf::Color::Green);
                break;
            } 
            } */  
            w.draw(unity);
        }  

        //now draws the edges
        Iter_Edge ei,ef;
        std::tie(ei,ef) = boost::edges(g);
        for (Iter_Edge e = ei; e != ef; e++)
        {
            Vertex vsource = boost::source(*e,g);
            Vertex vtarget = boost::target(*e,g); 
            int row_s = id_map[vsource]/SIDE;
            int column_s = id_map[vsource]-row_s*(SIDE);
            int row_t = id_map[vtarget]/SIDE;
            int column_t = id_map[vtarget]-row_t*(SIDE);
            
            sf::VertexArray lines(sf::LinesStrip, 2);
            
            sf::Vector2f starting_point = sf::Vector2f(0.25*display_height + (0.5+column_s)*L , 0.25*display_height + L*(row_s+0.5));
            sf::Vector2f target_point = sf::Vector2f(0.25*display_height + (0.5+column_t)*L , 0.25*display_height + L*(row_t+0.5));
            lines[0].position = starting_point;
            lines[1].position = target_point;
            int occ = 0;
            std::for_each(agents.begin(),agents.end(),[&](Agent const& a){if(a.get_edge()==*e){occ++;}});
            switch (occ)
            {
            case 0:
                lines[0].color = sf::Color::Black;
                lines[1].color = sf::Color::Black;
                break;
            case 1:
                lines[0].color = sf::Color::Yellow;
                lines[1].color = sf::Color::Yellow;
                break;
            case 2:
                lines[0].color = sf::Color::Red;
                lines[1].color = sf::Color::Red;
                break;
            case 3:
                lines[0].color = sf::Color::Green;
                lines[1].color = sf::Color::Green;
                break;
            default:
                lines[0].color = sf::Color::Blue;
                lines[1].color = sf::Color::Blue;
                break;
            } 

            w.draw(lines);
        }
              
    }


};
