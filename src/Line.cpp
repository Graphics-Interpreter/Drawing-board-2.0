#include <Line.hpp>
#include <functional>

namespace {
	std::vector<sf::Vector2i> DrawLine(sf::Vector2i startPosition, sf::Vector2i endPosition) {
	    std::vector<sf::Vector2i> vec2;
        int a = startPosition.y - endPosition.y,
	       b = endPosition.x - startPosition.x;
	    int d = 2 * a + b;
	    int a2b2 = 2 * (a + b), a2 = a * 2;
	    Vector2i current{startPosition};
        while (current.x < endPosition.x) {
            if (d < 0) {current.y++; d += a2b2;}
            else {d += a2;}
            current.x++; 
            vec2.push_back(current);
        }
	    return std::move(vec2);
	}
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    int deltaX = endPosition.x - startPosition.x;
    int deltaY = endPosition.y - startPosition.y;

    std::function<sf::Vector2i(sf::Vector2i)> Op;
    if (deltaY > 0 && deltaX > 0)
        Op = [](sf::Vector2i v)->sf::Vector2i{return v;};
    else if (deltaY >= 0 && deltaX <= 0)
        Op = [](sf::Vector2i v)->sf::Vector2i{v.x = -v.x; return v;};
    else if (deltaX >= 0 && deltaY <= 0)
        Op = [](sf::Vector2i v)->sf::Vector2i{v.y = -v.y; return v;};
    else
        Op = [](sf::Vector2i v)->sf::Vector2i{v = -v; return v;};

    std::function<sf::Vector2i(sf::Vector2i)> reverse;
    if (abs(deltaX) >= abs(deltaY))
    	reverse = [](Vector2i v)->sf::Vector2i {return v;};
    else
    	reverse = [](Vector2i v)->sf::Vector2i {std::swap(v.x, v.y); return v;};

	auto vec2 = DrawLine(reverse(Op(startPosition)), reverse(Op(endPosition)));

    std::vector<sf::Vertex> vertex;
    for (auto &v: vec2) 
    	vertex.push_back(sf::Vertex{sf::Vector2f{Op(reverse(v))}, sf::Color::Red});
    target.draw(&vertex[0], vertex.size(), sf::Points);
}