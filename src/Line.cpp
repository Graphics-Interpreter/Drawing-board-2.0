#include <Line.hpp>
#include <functional>
using namespace std;
using namespace sf;
using namespace graphics;

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
    target.draw(vertex, states);
}

std::shared_ptr<graphics::Shape> Line::cutBy(const std::vector<sf::Vertex> &scissor) {
    std::vector<sf::Vector2f> v;
    for (std::size_t pite = 0; pite + 1 < scissor.size(); pite++) {
        auto pbeg = scissor[pite].position, pdelta = scissor[pite + 1].position - scissor[pite].position;
        auto point = intersectionOf(pbeg, pdelta, start.position, end.position - start.position);
        if (point.x != -1 && point.y != -1) {
            //cout << Collinear(vbeg, vdelta, point) << " " << Collinear(vbeg, vdelta, point) << endl;
            v.push_back(point);
        }
    }
    auto vstart = start; vstart.position = *v.begin();
    auto vend = end; vend.position = *++v.begin();
    return make_shared<Line>(vstart, vend);
}

sf::Vector2f graphics::Shape::intersectionOf(sf::Vector2f p, sf::Vector2f r, sf::Vector2f q, sf::Vector2f s) {
    if (!crossProduct(r, s)) return sf::Vector2f{-1, -1};
    auto t = crossProduct((q - p), s) / crossProduct(r, s);
    auto u = crossProduct((q - p), r) / crossProduct(r, s);
    if (0 <= t && t <= 1 && 0 <= u && u <= 1)
        return sf::Vector2f{p.x + t * r.x, p.y + t * r.y};
    return sf::Vector2f{-1, -1};    
}

float graphics::Shape::crossProduct(sf::Vector2f v, sf::Vector2f w) {
    return v.x * w.y - v.y * w.x;
}