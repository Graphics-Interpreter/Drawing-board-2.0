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
	Polygon(const std::vector<sf::Vertex> &va):vertex{va} {}
	Polygon cutBy(const std::vector<sf::Vertex> &points);
	Polygon& SetTransform(const Transform &trans);
	sf::Vector2f getCenter();
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::vector<sf::Vertex> vertex;
};

const sf::Color Default = sf::Color::Red;
const sf::Color Scissor = sf::Color::Blue;