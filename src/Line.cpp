#include <Line.hpp>
#include <functional>
using namespace std;
using namespace sf;

namespace {
	vector<sf::Vector2f> DrawLine(sf::Vector2f start, sf::Vector2f end) {
	    std::vector<sf::Vector2f> vec2;
        int a = start.y - end.y,
	        b = end.x - start.x;
	    int d = 2 * a + b;
	    int a2b2 = 2 * (a + b), a2 = a * 2;
	    sf::Vector2f current{start};
        while (current.x <= end.x) {
            if (d < 0) {current.y++; d += a2b2;} else {d += a2;}
            current.x++; 
            vec2.push_back(current);
        }
	    return std::move(vec2);
	}
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    int deltaX = end.position.x - start.position.x;
    int deltaY = end.position.y - start.position.y;

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

	auto vec2 = DrawLine(reverse(Op(start.position)), reverse(Op(end.position)));

    sf::VertexArray vertex;
    for (auto &v: vec2) vertex.append(sf::Vertex{Op(reverse(v)), start.color});
    target.draw(vertex);
}