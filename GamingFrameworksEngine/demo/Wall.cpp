#include "Wall.h"

Wall::Wall()
{
	this->xAcceleration = 0;
	this->yAcceleration = 0;
	this->xPosition = 200;
	this->yPosition = 400;
	this->xSpeed = 0;
	this->ySpeed = 0;
	this->xSize = 600;
	this->hitbox = new Rectangle(xPosition, yPosition, xSize, ySize);
	this->shape = new Shape(4, xPosition, yPosition, xSize, ySize);
	this->shape->setColorFill(sf::Color::Green);
    this->material = new Material(0.0000017);
}