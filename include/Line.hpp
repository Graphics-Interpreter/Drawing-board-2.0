#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Line: public sf::Drawable {
public:
    Line(const sf::Vertex &s, const sf::Vertex &e)
	    :startPosition(s), endPosition(e) {}
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vertex startPosition, endPosition;
};


class Polygon: public sf::Drawable {
public:
	Polygon(sf::VertexArray &va):vertex{va} {}
	Polygon& SetTransform(const Transform &trans);
	Polygon& cutBy(const sf::VertexArray &points);
	sf::Vector2f getCenter();
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::VertexArray vertex;
	sf::Transform trans;
	sf::VertexArray intersection;
};