#include <list>
#include <iostream>
#include <math.h>
#include <map>
#include <Line.hpp>

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

    bool Collinear(sf::Vector2f p, sf::Vector2f r, sf::Vector2f point) {
        auto deltaY = point.y - p.y, deltaX = point.x - p.x;
        //cout << std::abs(r.x * deltaY - r.y * deltaX) << endl;
        //cout << deltaX / r.x << endl;
        if (std::abs(r.x * deltaY - r.y * deltaX) < 0.05
            && 0 < deltaY / r.y && deltaY / r.y < 1
            && 0 < deltaX / r.x && deltaX / r.x < 1) return true;
        else return false;
    }
}

Polygon Polygon::cutBy(const std::vector<sf::Vertex> &scissor) {
    std::vector<sf::Vertex> intersection;
    for (std::size_t vite = 0; vite + 1 < vertex.size(); vite++)
        for (std::size_t pite = 0; pite + 1 < scissor.size(); pite++) {
            auto vbeg = vertex[vite].position, vdelta = vertex[vite + 1].position - vertex[vite].position;
            auto pbeg = scissor[pite].position, pdelta = scissor[pite + 1].position - scissor[pite].position;
            auto point = intersect(vbeg, vdelta, pbeg, pdelta);
            if (point.x != -1 && point.y != -1) {
                //cout << Collinear(vbeg, vdelta, point) << " " << Collinear(vbeg, vdelta, point) << endl;
                point.x = ::round(point.x); point.y = ::round(point.y);
                intersection.push_back(sf::Vertex{point, sf::Color::Black});
            }
        }

    std::list<sf::Vertex> origin{vertex.begin(), vertex.end()};
    //std::list<sf::Vertex> shape{scissor.begin(), scissor.end()};
    for (size_t i = 0; i < intersection.size(); i++) {
        auto end = origin.begin(); auto beg = end++;
        while (end != origin.end()) {
            if (Collinear(beg->position, end->position - beg->position, intersection[i].position)) {
                origin.insert(end, sf::Vertex{intersection[i].position, Default});
                break;
            } else {
                beg++; end++;
            }
        }
    }
    std::vector<sf::Vertex> v{origin.begin(), origin.end()};
    return Polygon{v};
}

sf::Vector2f Polygon::getCenter() {
    sf::Vector2f center;
    for (std::size_t i = 0; i < vertex.size(); i++)
        center += vertex[i].position;
    center.x /= vertex.size();
    center.y /= vertex.size();
    return center;
}

Polygon& Polygon::SetTransform(const Transform &trans) {
    for (std::size_t i = 0; i < vertex.size(); i++) {
        Vector2f &pos = vertex[i].position;
        pos = trans.transformPoint(pos.x, pos.y);
    }
    return *this;
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (std::size_t i = 0; i + 1 < vertex.size(); i++)
        target.draw(Line(vertex[i], vertex[i + 1]));

    std::map<float, list<Edge>> edgeTable;
    float lineEnd = 0;
    for (std::size_t i = 0; i + 1 < vertex.size(); i++) {
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
        auto end = current.begin(); auto beg = end++;
        while (end != current.end()) {
            target.draw(Line(
                sf::Vertex{sf::Vector2f(::round(beg->posX), lineY), Default},
                sf::Vertex{sf::Vector2f(::round(end->posX), lineY), Default}));
            beg->posX += beg->delta; end->posX += end->delta;
            end++; if (end != current.end()) {beg = end; end++;}
        }
    }
}
