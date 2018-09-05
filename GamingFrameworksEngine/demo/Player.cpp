#include "Player.h"
#include <iostream>


Player::Player(std::list<Actor*>& actorList) : actorList(actorList)
{
	this->id = 0;
	this->objName = "Player";
	this->xPosition = 200;
	this->yPosition = 200;
	this->xSpeed = 0;
	this->ySpeed = 0;
	this->hitbox = new Rectangle(xPosition, yPosition, xSize, ySize);
	this->shape = new Shape(4, xPosition, yPosition, xSize, ySize);
	this->shape->setColorFill(sf::Color::White);
	this->buttons.push_back(Button(sf::Keyboard::Left, vector<double>(1,-.0005)));
	this->buttons.push_back(Button(sf::Keyboard::Right, vector<double>(1, .0005)));
	this->buttons.push_back(Button(sf::Keyboard::Up, vector<double>(1, -.0005)));
	vector<double> x = { 200, 200 };
	this->buttons.push_back(Button(sf::Keyboard::R, x));
}

Player::~Player()
{
    delete shape;
    delete hitbox;
}


void Player::step()
{
	//INPUTS HERE
	for (Button b : buttons) {
		b.updateState();
		if (b.isPressed()) {
			switch (b.getKey())
			{
			case sf::Keyboard::Left:
				this->setXSpeed(b.getParams()[0]);
				break;
			case sf::Keyboard::Right:
				this->setXSpeed(b.getParams()[0]);
				break;
			case sf::Keyboard::R:
				this->setXPosition(b.getParams()[0]);
				this->setYPosition(b.getParams()[1]);
				this->setXSpeed(0);
				this->setYSpeed(0);
				this->shape->setColorFill(sf::Color::White);
			default:
				break;
			}
		}

	}

	// Does joystick really need a class? Maybe player needs to know what joystick it is?
	// Or just joystick button bindings
	// joysticks could just "press" buttons..I dont know if thats wise though.
	if (sf::Joystick::isConnected(0)) {
		if (sf::Joystick::hasAxis(0, sf::Joystick::X)) {
			if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50) {
				this->setXSpeed(.0005);
			}
			else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -50) {
				this->setXSpeed(-.0005);
			}
		}
		if (sf::Joystick::isButtonPressed(0, 0)) {
			this->shape->setColorFill(sf::Color::Green);
		}
		if (sf::Joystick::isButtonPressed(0, 1)) {
			this->shape->setColorFill(sf::Color::Red);
		}
		if (sf::Joystick::isButtonPressed(0, 2)) {
			this->shape->setColorFill(sf::Color::Blue);
		}
		if (sf::Joystick::isButtonPressed(0, 3)) {
			this->shape->setColorFill(sf::Color::Yellow);
		}
		if (sf::Joystick::isButtonPressed(0, 7)) {
			this->setXPosition(200);
			this->setYPosition(200);
			this->setXSpeed(0);
			this->setYSpeed(0);
			this->shape->setColorFill(sf::Color::White);
		}

			
	}

	this->move(this->actorList);
	this->shape->setPosition(xPosition, yPosition);


}

void Player::draw(sf::RenderWindow * window, sf::View* view)
{
	float dx = xPosition - xPositionPrevious;
	float dy = yPosition - yPositionPrevious;
	view->move(dx*60, dy*80);
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
