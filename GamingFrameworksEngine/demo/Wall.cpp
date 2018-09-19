#include "Wall.h"

Wall::Wall(State startState) : Actor(startState)
{
	this->xAcceleration = 0;
	this->yAcceleration = 0;
	this->xSpeed = 0;
	this->ySpeed = 0;
	this->xSize = 600;
	this->ySize = 50;
	this->hitbox = new Rectangle(startState.xPosition, startState.yPosition, xSize, ySize);
	this->shape = new Shape(4, startState.xPosition, startState.yPosition, xSize, ySize);
	this->shape->setColorFill(sf::Color::Green);
    //this->material = new Material(0.017);
}