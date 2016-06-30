#ifndef _LINE_HPP_
#define _LINE_HPP_
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <functional>
#include <set>
#include <SFML/Graphics.hpp>

namespace graphics {
	class Shape: public sf::Drawable {
	public:
		virtual std::shared_ptr<Shape> cutBy(const std::vector<sf::Vertex> &points) = 0;
	protected:
		sf::Vector2f intersectionOf(sf::Vector2f p, sf::Vector2f r, sf::Vector2f q, sf::Vector2f s);
		float crossProduct(sf::Vector2f v, sf::Vector2f w);
    bool Collinear(sf::Vector2f p, sf::Vector2f r, sf::Vector2f point) {
        auto deltaY = point.y - p.y, deltaX = point.x - p.x;
        //cout << std::abs(r.x * deltaY - r.y * deltaX) << endl;
        //cout << deltaX / r.x << endl;
        if (std::abs(r.x * deltaY - r.y * deltaX) < 0.1
            && 0 < deltaY / r.y && deltaY / r.y < 1
            && 0 < deltaX / r.x && deltaX / r.x < 1) return true;
        else return false;
    }
	};

	class Line: public Shape {
	public:
    Line(const sf::Vertex &s, const sf::Vertex &e):Shape{}, start(s), end(e) {}
		~Line() {}
		std::shared_ptr<Shape> cutBy(const std::vector<sf::Vertex> &points) override;
		bool operator<(const Line &l) const {
		return start.position.x < l.start.position.x ||
				(!(l.start.position.x < start.position.x) && start.position.y < l.start.position.y);
		}
	private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		std::vector<sf::Vector2f> DrawSimpleLine(sf::Vector2f start, sf::Vector2f end) const;
    mutable sf::Vertex start, end;
	};

	class Circle: public Shape {
		public:
			Circle(const sf::Vertex &beg, const sf::Vertex &end) {
				auto point = beg.position + end.position; point.x /= 2, point.y /= 2;
				center.position = point, center.color = beg.color;
				radius = distance(beg.position, end.position) / 2;
			}
			Circle(const sf::Vertex &c, double r):center{c}, radius{r} {}
			~Circle(){}
			std::shared_ptr<Shape> cutBy(const std::vector<sf::Vertex> &points) override;
		private:
			std::vector<sf::Vector2f> Reverse(const std::vector<sf::Vector2f> &origin,
				const std::function<sf::Vector2f(sf::Vector2f)> &func) const;
			double distance(const sf::Vector2f &v1, const sf::Vector2f &v2) const {
				return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2));
			}
	    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			std::vector<sf::Vector2f> DrawSimpleCircle(double radius) const;
	    mutable sf::Vertex center;
			mutable double radius;
			mutable std::vector<sf::Vector2f> circle;
	};

	class Polygon: public Shape {
	public:
		Polygon(const std::vector<sf::Vertex> &va):vertex{va} {
			comp = [](const sf::Vector2f &v1, const sf::Vector2f &v2)->bool {
			    return v1.x < v2.x || (!(v2.x < v1.x) && v1.y < v2.y);
			};
		}
		Polygon(const Polygon &p):Shape{}, comp{p.comp}, vertex{p.vertex} {}
		Polygon(Polygon &&p):Shape{}, comp{std::move(p.comp)}, vertex{std::move(p.vertex)} {}
		Polygon& operator=(Polygon p) {using std::swap; comp = std::move(p.comp); swap(vertex, p.vertex); return *this;}
		~Polygon() {}

		std::shared_ptr<Shape> cutBy(const std::vector<sf::Vertex> &points) override;
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

	    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		Vfunc comp;
		mutable VArray vertex;
	};

	const sf::Color Default = sf::Color::Red;
	const sf::Color Scissor = sf::Color::Blue;
}
#endif
