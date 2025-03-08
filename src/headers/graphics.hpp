#ifndef GRAPHICS
#define GRAPHICS

#include<SFML/Graphics.hpp>
#include"alias.hpp"
#include"agents.hpp"
#include"config.hpp"

void inline print_info(Vertex v, Graph& dual)
{
    std::list<std::shared_ptr<Agent>>& queue = boost::get(&VertexProperty::queue, dual, v);
    if (queue.size() != 0)
    {
        std::cout << "VERTEX: " + std::to_string(dual[v].index) + " | agents id : ";
        std::for_each(queue.begin(), queue.end(), [&](std::shared_ptr<Agent>& a_ptr)
            { std::cout << a_ptr->get_id() << " ( to "; if (a_ptr->arrived()) { std::cout << "arr. "; }
            else { std::cout << dual[a_ptr->get_next_vertex()].index << "), "; };  });
        std::cout << std::endl << std::endl;
    }
}

void inline handle_events(std::unique_ptr<sf::RenderWindow>& m_main_window, bool& is_running, Graph& m_dual, bool& m_step) {
    sf::Event event;
    m_step = false;
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
                    case sf::Keyboard::P:
                        for(auto it = boost::vertices(m_dual).first; it != boost::vertices(m_dual).second ;it++){
                            print_info(*it, m_dual);
                        }
                        break;
                    case sf::Keyboard::N:
                        m_step = true;
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