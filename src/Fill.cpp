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
            last->posX += last->delta; ite->posX += ite->delta;
            target.draw(Line(
                sf::Vertex{sf::Vector2f(round(last->posX), lineY), sf::Color::Red},
                sf::Vertex{sf::Vector2f(round(ite->posX), lineY), sf::Color::Red}));
            ite++; if (ite != current.end()) {last = ite; ite++;}
        }
    }
}
