#include <iostream>
#include <SFML/Graphics.hpp>
#include <Line.hpp>
using namespace std;
using namespace sf;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Computer Graphics");

    sf::Vector2i pressPostion{-1, -1}, localPosition;
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && pressPostion.x == -1)
                if (event.mouseButton.button == sf::Mouse::Left) {
                    pressPostion.x = event.mouseButton.x;
                    pressPostion.y = event.mouseButton.y;
                }
            if (event.type == sf::Event::MouseButtonReleased)
                if (event.mouseButton.button == sf::Mouse::Left) {
                    pressPostion.x = -1;
                    pressPostion.y = -1;
                }
        }

        // clear the window with black color
        window.clear(sf::Color::White);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            localPosition = sf::Mouse::getPosition(window); 

            if (pressPostion.x != -1)
                window.draw(Line(pressPostion, localPosition));
        }
        // end the current frame
        window.display();
    }
    return 0;
}