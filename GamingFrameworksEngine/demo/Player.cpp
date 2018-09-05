#include "Player.h"

Player::Player(std::list<Actor*>& actorList) : actorList(actorList)
{
	this->id = 0;
	this->objName = "Player";
	this->xPosition = 200;
	this->yPosition = 200;
	this->xSpeed = 0.0005;
	this->hitbox = new Rectangle(xPosition, yPosition, xSize, ySize);
	this->shape = new Shape(4, xPosition, yPosition, xSize, ySize);
	this->shape->setColorFill(sf::Color::Blue);
}

Player::~Player()
{
    delete shape;
    delete hitbox;
}


void Player::step()
{
	//INPUTS HERE
	this->shape->setPosition(xPosition, yPosition);
	this->move(this->actorList);
}

void Player::draw(sf::RenderWindow * window)
{
	this->shape->draw(window);
}

void Player::onCollision(const Actor* other)
{
	if (other->getName() == "wall") {
		if (willCollideX(other)) {
			this->setXSpeed(0);
		}
		if (willCollideY(other)) {
			this->setYSpeed(0);
		}
	}
}
