#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include "Rectangle.h"

using namespace std;

// Holds an animation and a hitbox for objects to use.
class Sprite
{
private:
	Rectangle* hitbox;
	
	sf::Sprite s;
	sf::Texture t;
	sf::IntRect r;
	int xSize, ySize;
	int xPos, yPos;
	int numFrames;
public:
    Sprite(string filename, int xPos, int yPos, int xSize, int ySize);
    ~Sprite();

    Rectangle* getHitbox() const;

	void setPosition(int xPos, int yPos);
	void move(int dx, int dy);
	void rotate(int angle);
	void setSize(int sizeX, int sizeY);
	void setTexture(string filename, int xSize, int ySize);
	void draw(sf::RenderWindow* window);
};