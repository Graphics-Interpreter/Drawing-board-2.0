#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Line: public sf::Drawable {
public:
    Line(sf::Vector2i s, sf::Vector2i e):startPosition(s), endPosition(e){}
private:

    std::vector<sf::Vertex> FourthQ(bool Xaxies) const {
        std::vector<sf::Vertex> vertices;
        int a = startPosition.x - endPosition.x,
            b = -(endPosition.y - startPosition.y);
        if (Xaxies) {
            a = -(startPosition.y - endPosition.y);
            b = endPosition.x - startPosition.x;
        }
        int d = 2 * a + b;
        int a2b2 = 2 * (a + b), a2 = a * 2;
        Vector2i current{startPosition};
        if (Xaxies) {
            while (current.x < endPosition.x) {
                if (d < 0) {current.y--; d += a2b2;}
                else {d += a2;}
                current.x++; vertices.push_back(Vertex{sf::Vector2f(current),  sf::Color::Red});
            }
        } else {
            while (current.y > endPosition.y) {
                if (d < 0) {current.x++; d += a2b2;}
                else {d += a2;}
                current.y--; vertices.push_back(Vertex{sf::Vector2f(current),  sf::Color::Red});
            }
        }
        return std::move(vertices);
    }

    std::vector<sf::Vertex> FirstQ(bool Xaxies) const {
        std::vector<sf::Vertex> vertices;
        int a = startPosition.x - endPosition.x, 
            b = endPosition.y - startPosition.y;
        if (Xaxies) {
            a = startPosition.y - endPosition.y;
            b = endPosition.x - startPosition.x;
        }
        int d = 2 * a + b;
        int a2b2 = 2 * (a + b), a2 = a * 2;
        Vector2i current{startPosition};
        if (Xaxies) {
            while (current.x < endPosition.x) {
                if (d < 0) {current.y++; d += a2b2;}
                else {d += a2;}
                current.x++; vertices.push_back(Vertex{sf::Vector2f(current),  sf::Color::Red});
            }
        } else {
            while (current.y < endPosition.y) {
                if (d < 0) {current.x++; d += a2b2;}
                else {d += a2;}
                current.y++; vertices.push_back(Vertex{sf::Vector2f(current),  sf::Color::Red});
            }
        }
        return std::move(vertices);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        std::vector<sf::Vertex> vertices;
        int deltaX = endPosition.x - startPosition.x;
        int deltaY = endPosition.y - startPosition.y;
        if (deltaX > 0 && deltaY > 0) {
            if (deltaX >= deltaY) vertices = FirstQ(1);
            else vertices = FirstQ(0);
        } else if (deltaX > 0 && deltaY < 0) {
            if (abs(deltaX) >= abs(deltaY)) vertices = FourthQ(1);
            else vertices = FourthQ(0);
        }

        target.draw(&vertices[0], vertices.size(), sf::Points);
    }
    sf::Vector2i startPosition, endPosition;
};

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