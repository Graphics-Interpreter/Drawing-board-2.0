#include <Line.hpp>
#include <iostream>
#include <map>
#include <list>
using namespace std;
using namespace sf;

namespace {
	struct Edge {
	public:
		Edge(float x, float d, float sy, float y)
			:posX(x), startY(sy), endY(y), delta(d) {}
        float posX; // current X 
		float startY, endY;
		float delta;
	};

	// v1.y is v1
	Edge GetEdge(const sf::Vertex &v1, const sf::Vertex &v2) {
        auto smallp = v1.position, bigp = v2.position;
        if (smallp.y > bigp.y) swap(smallp, bigp);
		return Edge{smallp.x, (bigp.x - smallp.x) / (bigp.y - smallp.y), smallp.y, bigp.y};
	}

    inline float round(float f) { 
        return static_cast<float>(static_cast<int>(f + 0.5));
    }

    float crossProduct(sf::Vector2f v, sf::Vector2f w) {
        return v.x * w.y - v.y * w.x;
    }

    sf::Vector2f intersect(sf::Vector2f p, sf::Vector2f r,
        sf::Vector2f q, sf::Vector2f s) {
        if (!crossProduct(r, s)) return sf::Vector2f{-1, -1};
        auto t = crossProduct((q - p), s) / crossProduct(r, s);
        auto u = crossProduct((q - p), r) / crossProduct(r, s);
        if (0 <= t && t <= 1 && 0 <= u && u <= 1)
            return sf::Vector2f{p.x + t * r.x, p.y + t * r.y};
        return sf::Vector2f{-1, -1};    
    }
}

Polygon& Polygon::cutBy(const sf::VertexArray &points) {
    for (std::size_t i = 0; i + 1 < vertex.getVertexCount(); i++)
        for (std::size_t j = 0; j + 1 < points.getVertexCount(); j++) {
            auto point = intersect(vertex[i].position, vertex[i + 1].position - vertex[i].position,
                points[j].position, points[j + 1].position - points[j].position);
            if (point.x != -1 && point.y != -1) {
                //cout << point.x << " " << point.y << endl;
                intersection.append(sf::Vertex{point, sf::Color::Black});
            }
        }
    intersection.setPrimitiveType(sf::TrianglesFan);
    return *this;
}

sf::Vector2f Polygon::getCenter() {
    sf::Vector2f center;
    for (std::size_t i = 0; i < vertex.getVertexCount(); i++)
        center += vertex[i].position;
    center.x /= vertex.getVertexCount();
    center.y /= vertex.getVertexCount();
    return center;
}

Polygon& Polygon::SetTransform(const Transform &trans) {
    for (std::size_t i = 0; i < vertex.getVertexCount(); i++) {
        Vector2f &pos = vertex[i].position;
        pos = trans.transformPoint(pos.x, pos.y);
    }
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    std::map<float, list<Edge>> edgeTable;
    float lineEnd = 0;
    for (std::size_t i = 0; i + 1 < vertex.getVertexCount(); i++) {
        auto edge = GetEdge(vertex[i], vertex[i + 1]);
        edgeTable[edge.startY].push_back(edge);
        lineEnd = max(lineEnd, edge.endY);
    }

    std::list<Edge> current;
    for (int lineY = edgeTable.begin()->first; lineY < lineEnd; lineY++) {
        for (auto ite = current.begin(); ite != current.end();) {
            if (ite->endY <= lineY) ite = current.erase(ite);
            else ++ite;
        }
        if (edgeTable.count(lineY))
            for (auto &edge: edgeTable[lineY]) current.push_back(edge);
        auto comp = [](const Edge &e1, const Edge &e2)->bool {return e1.posX < e2.posX;};
        current.sort(comp);
        if (current.empty()) continue;
        auto ite = ++current.begin(), last = current.begin();
        while (ite != current.end()) {
            target.draw(Line(
                sf::Vertex{sf::Vector2f(round(last->posX), lineY), sf::Color::Red},
                sf::Vertex{sf::Vector2f(round(ite->posX), lineY), sf::Color::Red}));
            last->posX += last->delta; ite->posX += ite->delta;
            ite++; if (ite != current.end()) {last = ite; ite++;}
        }
    }
    //target.draw(intersection);
}
