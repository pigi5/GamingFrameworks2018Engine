#include "HUD.h"

HUD::HUD() {
	this->xPos = 20;
	this->yPos = 20;
	this->shape = new Shape(4, xPos, yPos);
	this->shape->setColorFill(sf::Color::Red);
}

void HUD::draw(sf::RenderWindow* window) {
	this->shape->draw(window);
}