#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Line.hpp>
using namespace std;
using namespace sf;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Computer Graphics");
    std::vector<sf::Vertex> points;
    std::shared_ptr<Polygon> ptr;
    sf::Color color = Default;
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // A new loop
                    if (color == Default && ptr) ptr = nullptr;
                    points.push_back(
                        sf::Vertex{sf::Vector2f{(float)event.mouseButton.x, (float)event.mouseButton.y}, color});
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    points.push_back(points.front());
                    if (color == Default) ptr = make_shared<Polygon>(points);
                    else ptr = make_shared<Polygon>(ptr->cutBy(points));
                    if (color == Default) color = Scissor; else color = Default; 
                    points.clear();
                } 
            }
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        if (ptr) window.draw(*ptr);
        for (std::size_t i = 0; i + 1 < points.size(); i++)
            window.draw(Line(points[i], points[i + 1]));

        sf::Vector2f currentPosition{sf::Mouse::getPosition(window)};
        if (!points.empty())
            window.draw(Line{points.back(), sf::Vertex{currentPosition, color}});
        window.display();
    }
    return 0;
}