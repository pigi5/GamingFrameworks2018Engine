#include "HUD.h"

HUD::HUD() {
	this->xPosition = 20;
	this->yPosition = 20;
	this->xSize = 300;
	this->ySize = 50;
	this->shape = new Shape(4, xPosition, yPosition, xSize, ySize);
	this->shape->setColorFill(sf::Color::Red);
}