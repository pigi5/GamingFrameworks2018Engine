#include "HUD.h"

HUD::HUD(State startState) : Overlay(startState)
{
	this->xSize = 300;
	this->ySize = 50;
	this->shape = new Shape(4, startState.xPosition, startState.yPosition, xSize, ySize);
	this->shape->setColorFill(sf::Color::Red);
}