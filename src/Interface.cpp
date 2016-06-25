#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Line.hpp>
using namespace std;
using namespace sf;
using namespace graphics;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Computer Graphics");
    std::vector<sf::Vertex> points;
    std::shared_ptr<graphics::Shape> ptr;
    sf::Color color = Default;
    sf::Vector2f lastPosition;
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
                    lastPosition = sf::Vector2f{static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y)};
                    points.push_back(sf::Vertex{lastPosition, color});
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (color == Default && points.size() > 2) {
                        points.push_back(points.front());
                        ptr = make_shared<Polygon>(points);
                    } else if (color == Default && points.size() == 2) {
                        ptr = make_shared<Line>(*points.begin(), *++points.begin());
                    } else {
                        points.push_back(points.front());
                        ptr = ptr->cutBy(points);
                    }
                    if (color == Default) color = Scissor; else color = Default; 
                    points.clear();
                } 
            }
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        sf::Vector2f currentPosition{sf::Mouse::getPosition(window)};
        if (ptr) {
            sf::Transform tran = sf::Transform::Identity;
            if (color == Default) tran.translate(currentPosition - lastPosition);
            window.draw(*ptr, tran);
        } 
        for (std::size_t i = 0; i + 1 < points.size(); i++)
            window.draw(Line(points[i], points[i + 1]));

        if (!points.empty())
            window.draw(Line{points.back(), sf::Vertex{currentPosition, color}});
        window.display();
    }
    return 0;
}