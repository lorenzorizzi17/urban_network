
#include"./alias.hpp"
#include"./agents.hpp"
#include<SFML/Graphics.hpp>


namespace boost{

    //std::map<int,sf::Color > map{{0,sf::Color::Black}, {2,sf::Color::Blue}, {1,sf::Color::Red},{3,sf::Color::Green},{4,sf::Color::Cyan}};
    
    void render_graph(sf::RenderWindow& w, Graph const& g, Graph const& dual, std::map<Vertex,Edge> dual_map,int SIDE,bool enable_dynamics){
        IdMap id_map = boost::get(boost::vertex_index,g);
        
        double R = 5;
        int display_height = w.getSize().x;
        double L = (display_height)/(SIDE);

        //draws the grid
        for (int cur_index =0; cur_index != SIDE*SIDE;cur_index++)
        {
            int row = cur_index/SIDE;
            int column = cur_index-row*(SIDE);
            
            sf::CircleShape unity(R);   
            unity.setPosition(0.015*display_height + (0.5+column)*L , 0.015*display_height + L*(row+0.5));
            unity.setOrigin(R/2,R/2);
            int occ = 0;
            unity.setFillColor(sf::Color::Black);
            w.draw(unity);
        }  

        //now draws the edges iterating over the dual graph
        Iter_Vertex vid,vfd;
        std::tie(vid,vfd) = boost::vertices(dual);
        for (Iter_Vertex v = vid; v != vfd; v++)
        {

            Vertex vsource = boost::source(dual_map.at(*v),g);
            Vertex vtarget = boost::target(dual_map.at(*v),g); 
            int row_s = id_map[vsource]/SIDE;
            int column_s = id_map[vsource]-row_s*(SIDE);
            int row_t = id_map[vtarget]/SIDE;
            int column_t = id_map[vtarget]-row_t*(SIDE);
            
            sf::VertexArray lines(sf::LinesStrip, 2);
            
            sf::Vector2f starting_point = sf::Vector2f(0.015*display_height + (0.5+column_s)*L , 0.015*display_height + L*(row_s+0.5));
            sf::Vector2f target_point = sf::Vector2f(0.015*display_height + (0.5+column_t)*L , 0.015*display_height + L*(row_t+0.5));
            lines[0].position = starting_point;
            lines[1].position = target_point;
            int occ = boost::get(boost::vertex_agents,dual,*v).size();
            if(occ == 0){
                lines[0].color = sf::Color::Black;
                lines[1].color = sf::Color::Black;
            } else if (occ < 2){
                lines[0].color = sf::Color::Blue;
                lines[1].color = sf::Color::Blue;
            } else if (occ < 5){ 
                lines[0].color = sf::Color::Black;
                lines[1].color = sf::Color::Black;
            } else if (occ < 10){
                lines[0].color = sf::Color::Green;
                lines[1].color = sf::Color::Green;
            } else if (occ < 15){
                lines[0].color = sf::Color::Red;
                lines[1].color = sf::Color::Red;
            } else if (occ < 30){
                lines[0].color = sf::Color::Yellow;
                lines[1].color = sf::Color::Yellow;
            }
                

            w.draw(lines);
        }
              
    }

//STILL WIP
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
