#include"Button.h"
using namespace std;

Button::Button(GameCommand command, sf::Keyboard::Key key) {
	this->command = command;
	this->key = key;
}

Button::~Button(){
}

void Button::setCommand(GameCommand command){
	this->command = command;
}

void Button::setKey(sf::Keyboard::Key key){
	this->key = key;
}

void Button::setState(bool state){
	this->state = state;
}

bool Button::isPressed(){
	return state;
}

void Button::updateState(){
	if (sf::Keyboard::isKeyPressed(this->key)) {
		setState(true);
	}
	else {
		setState(false);
	}
}
