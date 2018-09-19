#include "Player.h"
#include <iostream>


Player::Player(State startState) : Actor(startState)
{
	this->xSize = 30;
    this->ySize = 30;
    this->startState = startState;
	this->id = 0;
	this->objName = "Player";
	this->maxXSpeed = 2;
	this->maxYSpeed = 10;
	this->hitbox = new Rectangle(startState.xPosition, startState.yPosition, xSize, ySize);
	this->shape = new Shape(4, startState.xPosition, startState.yPosition, xSize, ySize);
	this->shape->setColorFill(sf::Color::White);
	this->buttons.push_back(Button(sf::Keyboard::Left, vector<double>(1,-0.03)));
	this->buttons.push_back(Button(sf::Keyboard::Right, vector<double>(1, 0.03)));
	this->buttons.push_back(Button(sf::Keyboard::Up, vector<double>(1, -.0005), "../demoSound.wav"));
	vector<double> x = { 200, 200 };
	this->buttons.push_back(Button(sf::Keyboard::R, x));
}

void Player::step()
{
	//INPUTS HERE
	this->setXAcceleration(0);
	for (Button b : buttons) {
		b.updateState();
		if (b.isPressed()) {
			switch (b.getKey())
			{
			case sf::Keyboard::Left:
				this->setXAcceleration(b.getParams()[0]);
				break;
			case sf::Keyboard::Right:
				this->setXAcceleration(b.getParams()[0]);
				break;
			case sf::Keyboard::R:
				this->nextState = startState;
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
				this->setXSpeed(.5);
			}
			else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -50) {
				this->setXSpeed(-.5);
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
			this->nextState = startState;
			this->setXSpeed(0);
			this->setYSpeed(0);
			this->shape->setColorFill(sf::Color::White);
		}
	}
}

void Player::draw(sf::RenderWindow* window, sf::View* view)
{
	view->setCenter(currentState.xPosition, currentState.yPosition);
	Actor::draw(window, view);
}
