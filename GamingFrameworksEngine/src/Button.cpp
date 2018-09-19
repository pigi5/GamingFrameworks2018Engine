#include"../header/Button.h"
using namespace std;

Button::Button() {
	this->key = sf::Keyboard::A;
}

Button::Button(sf::Keyboard::Key key, vector<double> params) {
	this->key = key;
	this->params = params;
}

Button::Button(sf::Keyboard::Key key, vector<double> params, string soundFile) {
	this->key = key;
	this->params = params;
	this->sound = Sound(soundFile);
	if (this->sound.loadSound())
	{
		this->hasSound = true;
	}
}


Button::~Button(){
}


void Button::setKey(sf::Keyboard::Key key){
	this->key = key;
}

sf::Keyboard::Key Button::getKey(){
	return key;
}

void Button::setParams(vector<double> params){
	this->params = params;
}

vector<double> Button::getParams(){
	return params;
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
		if (this->hasSound) {
			this->sound.playSound();
		}
	}
	else {
		setState(false);
	}
}

