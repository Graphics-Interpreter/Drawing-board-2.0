#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

void FillPolygon(sf::RenderTarget& target, const std::vector<sf::Vector2i>&);

class Line: public sf::Drawable {
public:
    Line(sf::Vector2i s, sf::Vector2i e):startPosition(s), endPosition(e) {}
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vector2i startPosition, endPosition;
};
