#pragma once
#include <SFML\Graphics.hpp>

using namespace std;

class Shape {
private:
	sf::CircleShape s1;
	sf::RectangleShape s2;
	bool sType;
	int xPos, yPos;
	int xSize, ySize;
	int numPoints;
	sf::Color fillColor;
	sf::Color outlineColor;

public:
	Shape(int numPoints, int xPos, int yPos);
	void setPosition(int xPos, int yPos);
	void move(int dx, int dy);
	void rotate(int angle);
	void setSize(int xSize, int ySize);
	void setColorFill(sf::Color color);
	void setColorOutline(sf::Color color);
	void setOutlineThickness(int size);
	void setTexture(string filename, int xSize, int ySize);
	void draw(sf::RenderWindow* window);
};