#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <set>
#include <SFML/Graphics.hpp>

class Line: public sf::Drawable {
public:
    Line(const sf::Vertex &s, const sf::Vertex &e):Drawable{}, start(s), end(e) {}
	~Line() {}
	bool operator<(const Line &l) {
		return start.position.x < l.start.position.x || 
			(l.start.position.x < start.position.x && start.position.y < l.start.position.y);
	}
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    mutable sf::Vertex start, end;
};


class Polygon: public sf::Drawable {
public:
	Polygon(const std::vector<sf::Vertex> &va):vertex{va} {
		comp = [](const sf::Vertex &v1, const sf::Vertex &v2)->bool {
		    return v1.position.x < v2.position.x || ((v2.position.x < v1.position.x) && v1.position.y < v2.position.y);
		};
	}
	Polygon(const Polygon &p):Drawable{}, comp{p.comp}, vertex{p.vertex} {}
	Polygon(Polygon &&p):Drawable{}, comp{std::move(p.comp)}, vertex{std::move(p.vertex)} {}
	Polygon& operator=(Polygon p) {using std::swap; comp = std::move(p.comp); swap(vertex, p.vertex); return *this;}
	~Polygon() {}

	Polygon cutBy(const std::vector<sf::Vertex> &points);
	Polygon& SetTransform(const sf::Transform &trans);
	typedef std::vector<sf::Vertex> VArray;
	sf::Vector2f getCenter();
private:
	typedef std::function<bool(const sf::Vertex &v1, const sf::Vertex &v2)> Vfunc;
	VArray insertInto(const Polygon::VArray &vertex, const std::set<sf::Vertex, Vfunc> &intersection);
	std::map<Line, VArray> setSegment(VArray origin, const std::set<sf::Vertex, Vfunc> &intersection);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Vfunc comp;
	mutable VArray vertex;
};

const sf::Color Default = sf::Color::Red;
const sf::Color Scissor = sf::Color::Blue;
const sf::Color Intersection = sf::Color::Black;