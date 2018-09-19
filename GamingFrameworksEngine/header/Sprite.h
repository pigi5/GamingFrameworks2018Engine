#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include "Hitbox.h"

using namespace std;

// Holds an animation and a hitbox for objects to use.
class Sprite
{
private:
	Hitbox* hitbox;
	
	sf::Sprite s;
	sf::Texture* t;
	sf::IntRect r;
	int xSize, ySize;
	int xPos, yPos;
	int numFrames;
public:
	Sprite();
    Sprite(int xPos, int yPos);
    ~Sprite();

    Hitbox* getHitbox() const;

	void setPosition(int xPos, int yPos);
	void move(int dx, int dy);
	void rotate(int angle);
	void setSize(int sizeX, int sizeY);
	void setTexture(string filename);
	void setPartialTexture(string filename, int xSize, int ySize);
	vector<sf::Sprite> parseSpriteSheet(string filename, int xSize, int ySize, int num);
	void draw(sf::RenderWindow* window);
};