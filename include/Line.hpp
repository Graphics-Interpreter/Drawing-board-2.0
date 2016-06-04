#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

namespace {
	std::vector<sf::Vertex> FirstQ(bool Xaxies, sf::Vector2i startPosition,
	    sf::Vector2i endPosition);
	std::vector<sf::Vertex> FirstQ
	    (sf::Vector2i startPosition, sf::Vector2i endPosition);
}

class Line: public sf::Drawable {
public:
    Line(sf::Vector2i s, sf::Vector2i e):startPosition(s), endPosition(e) {}
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vector2i startPosition, endPosition;
};

