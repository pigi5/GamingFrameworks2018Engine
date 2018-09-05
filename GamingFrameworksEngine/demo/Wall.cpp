#include "Wall.h"

Wall::Wall(std::list<Actor*>& actorList) : actorList(actorList){
	this->xAcceleration = 0;
	this->yAcceleration = 0;
	this->xPosition = 200;
	this->yPosition = 400;
	this->xSpeed = 0;
	this->ySpeed = 0;
	this->sprite = NULL;
	this->shape = new Shape(4, xPosition, yPosition);
	this->shape->setColorFill(sf::Color::Green);
}
void Wall::step() {
	this->move(actorList);
}
void Wall::draw(sf::RenderWindow* window) {
	this->shape->draw(window);
}
void Wall::onCollision(const Actor* other) {

}