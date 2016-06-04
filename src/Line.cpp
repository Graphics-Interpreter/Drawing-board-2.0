#include <Line.hpp>

namespace {
	std::vector<sf::Vertex> FirstQ(bool Xaxies, sf::Vector2i startPosition,
	    sf::Vector2i endPosition) {
	    std::vector<sf::Vertex> vertices;
	    int a = startPosition.x - endPosition.x, 
	        b = endPosition.y - startPosition.y;
	    if (Xaxies) {
	        a = startPosition.y - endPosition.y;
	        b = endPosition.x - startPosition.x;
	    }
	    int d = 2 * a + b;
	    int a2b2 = 2 * (a + b), a2 = a * 2;
	    Vector2i current{startPosition};
	    if (Xaxies) {
	        while (current.x < endPosition.x) {
	            if (d < 0) {current.y++; d += a2b2;}
	            else {d += a2;}
	            current.x++; vertices.push_back(Vertex{sf::Vector2f(current),  sf::Color::Red});
	        }
	    } else {
	        while (current.y < endPosition.y) {
	            if (d < 0) {current.x++; d += a2b2;}
	            else {d += a2;}
	            current.y++; vertices.push_back(Vertex{sf::Vector2f(current),  sf::Color::Red});
	        }
	    }
	    return std::move(vertices);
	}

	std::vector<sf::Vertex> FirstQ
	    (sf::Vector2i startPosition, sf::Vector2i endPosition) {
	    int deltaX = endPosition.x - startPosition.x;
	    int deltaY = endPosition.y - startPosition.y;
	    if (deltaX >= deltaY) return std::move(FirstQ(1, startPosition, endPosition));
	    return std::move(FirstQ(0, startPosition, endPosition));
	}
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    std::vector<sf::Vertex> vertices;
    int deltaX = endPosition.x - startPosition.x;
    int deltaY = endPosition.y - startPosition.y;

    if (deltaY > 0 && deltaX > 0) {
        vertices = FirstQ(startPosition, endPosition);
    } else if (deltaY > 0 && deltaX < 0) {
        sf::Vector2i start = startPosition, end = endPosition;
        end.x = -end.x;
        start.x = -start.x;
        vertices = FirstQ(start, end);
        for (auto &v: vertices) v.position.x = -v.position.x;
    } else if (deltaX > 0 && deltaY < 0) {
        sf::Vector2i start = startPosition, end = endPosition;
        end.y = -end.y;
        start.y = -start.y;
        vertices = FirstQ(start, end);
        for (auto &v: vertices) v.position.y = -v.position.y;
    } else {
        sf::Vector2i start = -startPosition, end = -endPosition;
        vertices = FirstQ(start, end);
        for (auto &v: vertices) v.position = -v.position;
    }

    target.draw(&vertices[0], vertices.size(), sf::Points);
}