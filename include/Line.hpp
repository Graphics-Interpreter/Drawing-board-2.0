#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

inline sf::Vertex makeVertex(const sf::Vector2i &v2, 
	const sf::Color color = sf::Color::Red) {
	return sf::Vertex(sf::Vector2f{
		static_cast<float>(v2.x), static_cast<float>(v2.y)}, color);
}

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
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::VertexArray vertex;
};