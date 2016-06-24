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
	bool operator<(const Line &l) const {
		return start.position.x < l.start.position.x || 
			(!(l.start.position.x < start.position.x) && start.position.y < l.start.position.y);
	}
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    mutable sf::Vertex start, end;
};


class Polygon: public sf::Drawable {
public:
	Polygon(const std::vector<sf::Vertex> &va):vertex{va} {
		comp = [](const sf::Vector2f &v1, const sf::Vector2f &v2)->bool {
		    return v1.x < v2.x || (!(v2.x < v1.x) && v1.y < v2.y);
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
	typedef std::function<bool(const sf::Vector2f &v1, const sf::Vector2f &v2)> Vfunc;
    static const size_t ORIGIN = 0, SCISSOR = 1;

	VArray insertInto(const Polygon::VArray &vertex, const std::set<sf::Vector2f, Vfunc> &intersection);
	std::map<Line, VArray> setSegment(VArray origin, const std::set<sf::Vector2f, Vfunc> &intersection);
	std::map<Line, Polygon::VArray> getOrderSeg(const VArray &scissor, const VArray &correctOrder,
	    const std::set<sf::Vector2f, Vfunc> &intersection);
	size_t getStartIndex(const VArray& scissor, const sf::Vector2f &v);
	std::set<sf::Vector2f, Vfunc> getIntersection(const VArray &scissor);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Vfunc comp;
	mutable VArray vertex;
};

const sf::Color Default = sf::Color::Red;
const sf::Color Scissor = sf::Color::Blue;