#include <Line.hpp>
#include <functional>
using namespace std;
using namespace sf;

namespace {
	vector<sf::Vector2f> DrawLine(sf::Vector2f startPosition, sf::Vector2f endPosition) {
	    std::vector<sf::Vector2f> vec2;
        int a = startPosition.y - endPosition.y,
	        b = endPosition.x - startPosition.x;
	    int d = 2 * a + b;
	    int a2b2 = 2 * (a + b), a2 = a * 2;
	    sf::Vector2f current{startPosition};
        while (current.x <= endPosition.x) {
            if (d < 0) {current.y++; d += a2b2;} else {d += a2;}
            current.x++; 
            vec2.push_back(current);
        }
	    return std::move(vec2);
	}
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    int deltaX = endPosition.position.x - startPosition.position.x;
    int deltaY = endPosition.position.y - startPosition.position.y;

    std::function<sf::Vector2f(sf::Vector2f)> Op;
    if (deltaY >= 0 && deltaX >= 0)
        Op = [](sf::Vector2f v)->sf::Vector2f{return v;};
    else if (deltaY > 0 && deltaX < 0)
        Op = [](sf::Vector2f v)->sf::Vector2f{v.x = -v.x; return v;};
    else if (deltaX > 0 && deltaY < 0)
        Op = [](sf::Vector2f v)->sf::Vector2f{v.y = -v.y; return v;};
    else
        Op = [](sf::Vector2f v)->sf::Vector2f{v= -v; return v;};

    std::function<sf::Vector2f(sf::Vector2f)> reverse;
    if (abs(deltaX) > abs(deltaY)) {
    	reverse = [](sf::Vector2f v)->sf::Vector2f {return v;};
    } else {
    	reverse = [](sf::Vector2f v)->sf::Vector2f {std::swap(v.x, v.y); return v;};
    }

	auto vec2 = DrawLine(reverse(Op(startPosition.position)), reverse(Op(endPosition.position)));

    sf::VertexArray vertex;
    for (auto &v: vec2) vertex.append(sf::Vertex{Op(reverse(v)), startPosition.color});
    target.draw(vertex);
}