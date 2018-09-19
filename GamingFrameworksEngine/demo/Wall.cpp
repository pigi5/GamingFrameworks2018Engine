#include "Wall.h"

Wall::Wall(State startState) : Actor(startState)
{
	this->xAcceleration = 0;
	this->yAcceleration = 0;
	this->xSpeed = 0;
	this->ySpeed = 0;
	this->xSize = 600;
	this->ySize = 50;
	this->sprite = new Sprite(startState.xPosition, startState.yPosition);
	this->sprite->setPartialTexture("./demo/wall.png", xSize, ySize);
	this->hitbox = new Hitbox(startState.xPosition, startState.yPosition, xSize, ySize);
    //this->material = new Material(0.017);
}