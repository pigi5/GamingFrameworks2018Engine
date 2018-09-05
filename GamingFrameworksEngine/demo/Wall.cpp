#include "Wall.h"

Wall::Wall(std::list<Actor*>& actorList) : actorList(actorList)
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
Wall::~Wall()
{
    delete shape;
    delete hitbox;
    delete material;
}

void Wall::step()
{
	this->move(actorList);
}
void Wall::draw(sf::RenderWindow* window, sf::View* view)
{
	this->shape->draw(window);
}
void Wall::onCollision(const Actor* other)
{

}