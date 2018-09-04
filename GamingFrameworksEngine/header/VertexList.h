#pragma once
#include <vector>
#include <SFML\Graphics.hpp>

using namespace std;

class VertexList {
private:
	sf::VertexArray va;
	sf::PrimitiveType type;
	int count;

public:
	VertexList(sf::PrimitiveType type, int vertexCount, sf::Vertex vList[]);
	void changeType(sf::PrimitiveType type);
	void changeVertexPosition(int index, int posX, int posY);
	void changeVertexColor(int index, sf::Color color);
	void addVertex(sf::Vertex v);
	void draw(sf::RenderWindow* window);
};