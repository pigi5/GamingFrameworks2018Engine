#include "../header/Shape.h"

Shape::Shape(int numPoints, int xPos, int yPos, int xSize, int ySize) {
	this->xPos = xPos;
	this->yPos = yPos;
	this->numPoints = numPoints;
	this->xSize = xSize;
	this->ySize = ySize;
	this->fillColor = sf::Color();
	this->outlineColor = sf::Color();
	if (numPoints == 4) {
		sf::RectangleShape s = sf::RectangleShape(sf::Vector2f(xSize, ySize));
		s.setPosition(xPos, yPos);
		s2 = s;
	}
	else {
		sf::CircleShape s = sf::CircleShape(xSize, numPoints);
		s.setPosition(xPos, yPos);
		s1 = s;
	}
}
void Shape::setPosition(int xPos, int yPos) {
	this->xPos = xPos;
	this->yPos = yPos;
	s1.setPosition(xPos, yPos);
	s2.setPosition(xPos, yPos);
}
void Shape::move(int dx, int dy) {
	this->xPos += dx;
	this->yPos += dy;
	s1.move(dx, dy);
	s2.move(dx, dy);
}
void Shape::rotate(int angle) {
	s1.rotate(angle);
	s2.rotate(angle);
}
void Shape::setSize(int xSize, int ySize) {
	s1.setScale(xSize, ySize);
	s2.setScale(xSize, ySize);
}
void Shape::setColorFill(sf::Color color) {
	this->fillColor = color;
	s1.setFillColor(color);
	s2.setFillColor(color);
}
void Shape::setColorOutline(sf::Color color) {
	this->outlineColor = color;
	s1.setOutlineColor(color);
	s2.setOutlineColor(color);
}
void Shape::setOutlineThickness(int size) {
	s1.setOutlineThickness(size);
	s2.setOutlineThickness(size);
}
void Shape::setTexture(string filename, int xSize, int ySize) {
	this->xSize = xSize;
	this->ySize = ySize;
	sf::Texture* t = new sf::Texture();
	sf::IntRect r = sf::IntRect(sf::Vector2i(xPos, yPos), sf::Vector2i(xSize, ySize));
	t->loadFromFile(filename, r);
	s1.setTexture(t, true);
	s2.setTexture(t, true);
}

void Shape::draw(sf::RenderWindow* window) {
	if (numPoints != 4) {
		window->draw(s1);
	}
	else {
		window->draw(s2);
	}
}