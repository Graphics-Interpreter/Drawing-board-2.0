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
    // current point set
    sf::VertexArray points;
    // current position
    sf::Vector2i lastPosition;
    // Polygon shape
    std::shared_ptr<Polygon> ptr;
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto color = sf::Color::Red;
                    if (ptr) color = sf::Color::Blue;
                    points.append(
                        sf::Vertex{sf::Vector2f{event.mouseButton.x, event.mouseButton.y}, color});
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    points.append(points[0]);
                    if (!ptr) {ptr = make_shared<Polygon>(points); points.clear();}
                    else {ptr->cutBy(points);}
                    lastPosition = sf::Mouse::getPosition(window);
                } 
            }
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        if (ptr) window.draw(*ptr);
        for (std::size_t i = 0; i + 1 < points.getVertexCount(); i++)
            window.draw(Line(points[i], points[i + 1]));

        sf::Vector2f currentPosition{sf::Mouse::getPosition(window)};
        auto color = sf::Color::Red;
        if (ptr) color = sf::Color::Blue;
        if (points.getVertexCount())
            window.draw(Line{points[points.getVertexCount() - 1], sf::Vertex{currentPosition, color}});
        window.display();
    }
    return 0;
}