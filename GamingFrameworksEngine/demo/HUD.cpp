#include "HUD.h"

HUD::HUD() {
	this->xPos = 20;
	this->yPos = 20;
	this->xSize = 300;
	this->ySize = 50;
	this->shape = new Shape(4, xPos, yPos, xSize, ySize);
	this->shape->setColorFill(sf::Color::Red);
}

HUD::~HUD() {
    delete shape;
}

void HUD::draw(sf::RenderWindow* window) {
	this->shape->draw(window);
}