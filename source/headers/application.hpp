#include<SFML/Graphics.hpp>


class Application{
    private:
    sf::RenderWindow m_window;
    int m_fps = 60;
    int m_display_height;
    

    public:
    Application() : m_window(sf::RenderWindow(sf::VideoMode(m_display_height, m_display_height), "Simulation", sf::Style::Default)){
        m_window.setFramerateLimit(m_fps);
    }
};