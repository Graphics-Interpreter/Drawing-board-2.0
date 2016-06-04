#include <iostream>
#include <SFML/Graphics.hpp>
#include <Line.hpp>
using namespace std;
using namespace sf;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Computer Graphics");
    std::vector<sf::Vector2i> points;
    sf::Vector2i pressPostion, localPosition;
    bool valid = false;
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!valid) points.clear(); valid = true; 
                    pressPostion.x = event.mouseButton.x;
                    pressPostion.y = event.mouseButton.y;
                    points.push_back(pressPostion);
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    valid = false; points.push_back(points.front());
                    // TODO
                }
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        if (valid) {
            localPosition = sf::Mouse::getPosition(window); 
            if (!points.empty())
                window.draw(Line(points.back(), localPosition));
        }
        for (int i = 0; i + 1 < points.size(); i++)
            window.draw(Line(points[i], points[i + 1]));
        // end the current frame
        window.display();
    }
    return 0;
}