#include "../header/VertexList.h"

VertexList::VertexList(sf::PrimitiveType type, int vertexCount, sf::Vertex vList[]) {
	this->va = sf::VertexArray(type, vertexCount);
	this->type = type;
	this->count = vertexCount;
	for (int i = 0; i < vertexCount; i++) {
		va[i] = vList[i];
	}
}
void VertexList::changeType(sf::PrimitiveType type) {
	va.setPrimitiveType(type);
}
void VertexList::changeVertexPosition(int index, int posX, int posY) {
	va[index].position.x = posX;
	va[index].position.y = posY;
}
void VertexList::changeVertexColor(int index, sf::Color color) {
	va[index].color = color;
}
void VertexList::addVertex(sf::Vertex v) {
	va.append(v);
}

void VertexList::draw(sf::RenderWindow* window) {
	window->draw(va);
}