#include <iostream>
#include <SFML/Graphics.hpp>
#include <Line.hpp>
using namespace std;
using namespace sf;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Computer Graphics");
    // current point set
    sf::VertexArray points;
    // finish?
    bool valid = false;
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
                    if (!valid) points.clear(); 
                    valid = true; 
                    points.append(makeVertex(sf::Vector2i{event.mouseButton.x, event.mouseButton.y}));
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    valid = false; points.append(points[0]);
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        if (points.getVertexCount()) {
            for (std::size_t i = 0; i + 1 < points.getVertexCount(); i++)
                window.draw(Line(points[i], points[i + 1]));
            if (valid)
                window.draw(Line{
                   points[points.getVertexCount() - 1],
                   makeVertex(sf::Mouse::getPosition(window))});
            else
                window.draw(Polygon{points});
        }
        window.display();
    }
    return 0;
}