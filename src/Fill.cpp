#include <Line.hpp>
#include <iostream>
#include <map>
#include <list>
using namespace std;
using namespace sf;

namespace {
	struct Edge {
	public:
		Edge(int x, double d, int sy, int y)
			:posX(x), delta(d), startY(sy), endY(y) {}
		int posX; int endY;
		int startY;
		double delta;
	};

	// small.y is small
	Edge GetEdge(const sf::Vector2i &small, const sf::Vector2i &bigger) {
		return Edge{small.x, (double)(bigger.x - small.x) / (bigger.y - small.y),
			small.y, bigger.y};
	}
}

void FillPolygon(sf::RenderTarget& target, const std::vector<sf::Vector2i> &points) {
	std::map<int, list<Edge>> edgeTable;
    int lineEnd = 0;
    for (int i = 0; i + 1 < points.size(); i++)
    	if (points[i].y < points[i + 1].y) {
    		edgeTable[points[i].y].push_back(GetEdge(points[i], points[i + 1]));
    		lineEnd = max(lineEnd, points[i + 1].y);
    	} else {
    		edgeTable[points[i + 1].y].push_back(GetEdge(points[i + 1], points[i]));
    		lineEnd = max(lineEnd, points[i].y);
    	}

    std::list<Edge> current;
    for (int lineY = edgeTable.begin()->first; lineY < lineEnd; lineY++) {
    	for (auto ite = current.begin(); ite != current.end();) {
    		//ite->posX = ite->delta * (lineY - ite->startY);
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
    			sf::Vector2i(last->posX + last->delta * (lineY - last->startY), lineY),
	    		sf::Vector2i(ite->posX + ite->delta * (lineY - ite->startY), lineY)));
			last = ite; ite++;

    	}
    }
}