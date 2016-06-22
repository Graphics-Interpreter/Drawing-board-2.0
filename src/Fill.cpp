#include <list>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <math.h>
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

    inline sf::Vector2f round(const sf::Vector2f &p) { 
        return sf::Vector2f{(float)(int)(p.x + 0.5), (float)(int)(p.y + 0.5)};
    }

    float crossProduct(sf::Vector2f v, sf::Vector2f w) {
        return v.x * w.y - v.y * w.x;
    }

    sf::Vector2f intersectionOf(sf::Vector2f p, sf::Vector2f r,
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
        if (std::abs(r.x * deltaY - r.y * deltaX) < 0.1
            && 0 < deltaY / r.y && deltaY / r.y < 1
            && 0 < deltaX / r.x && deltaX / r.x < 1) return true;
        else return false;
    }

    void printV(const sf::Vector2f &v) {
        std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
    }
}

std::map<Line, Polygon::VArray> Polygon::setSegment(VArray origin, 
    const std::set<sf::Vector2f, Vfunc> &intersection) {
    cout << endl;
    auto predicate = [&](const sf::Vertex &v) {return intersection.count(v.position);};
    for (auto v = ++origin.begin(); v != origin.end(); v++) {
        origin.push_back(*v); if (predicate(*v)) break;
    }
    #ifdef DEBUG
    for (auto &v: origin) printV(v.position);
    cout << "--" << endl;
    for (auto &v: intersection) printV(v);
    #endif

    std::map<Line, Polygon::VArray> res;
    auto beg = origin.begin(), end = origin.begin();
    if ((beg = std::find_if(origin.begin(), origin.end(), predicate)) != origin.end())
        while ((end = std::find_if(beg + 1, origin.end(), predicate)) != origin.end()) {
            #ifdef DEBUG
            printV(beg->position);
            #endif
            for (auto ite = beg + 1; ite != end; ite++) {
                res[Line{*beg, *end}].push_back(*ite);
                #ifdef DEBUG
                printV(ite->position);
                #endif
            }
            #ifdef DEBUG
            printV(end->position);
            #endif
            beg = end;
        }
    return res;
}

Polygon::VArray Polygon::insertInto(const Polygon::VArray &vertex, 
    const std::set<sf::Vector2f, Vfunc> &intersection) {
    std::list<sf::Vertex> origin{vertex.begin(), vertex.end()};
    for (const auto &v: intersection) {
        auto end = origin.begin(); auto beg = end++;
        while (end != origin.end()) {
            if (Collinear(beg->position, end->position - beg->position, v)) {
                origin.insert(end, sf::Vertex{v, Default}); break;
            } else {
                beg++; end++;
            }
        }
    }
    return std::move(VArray{origin.begin(), origin.end()});
}


Polygon Polygon::cutBy(const VArray &scissor) {
    std::set<sf::Vector2f, Vfunc> intersection(comp);
    for (std::size_t vite = 0; vite + 1 < vertex.size(); vite++)
        for (std::size_t pite = 0; pite + 1 < scissor.size(); pite++) {
            auto vbeg = vertex[vite].position, vdelta = vertex[vite + 1].position - vertex[vite].position;
            auto pbeg = scissor[pite].position, pdelta = scissor[pite + 1].position - scissor[pite].position;
            auto point = intersectionOf(vbeg, vdelta, pbeg, pdelta);
            if (point.x != -1 && point.y != -1) {
                //cout << Collinear(vbeg, vdelta, point) << " " << Collinear(vbeg, vdelta, point) << endl;
                intersection.insert(point);
            }
        }
    std::map<Line, std::vector<sf::Vertex>> segment[2];
    segment[0] = setSegment(insertInto(vertex, intersection), intersection);
    segment[1] = setSegment(insertInto(scissor, intersection), intersection);
    return Polygon{scissor};
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
    for (auto &v: vertex) v.position = round(v.position);
    /*
    for (std::size_t i = 0; i + 1 < vertex.size(); i++) {
        target.draw(Line(vertex[i], vertex[i + 1]));
    }
    */
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
                sf::Vertex{sf::Vector2f(beg->posX, lineY), Default},
                sf::Vertex{sf::Vector2f(end->posX, lineY), Default}));
            beg->posX += beg->delta; end->posX += end->delta;
            end++; if (end != current.end()) {beg = end; end++;}
        }
    }
}
