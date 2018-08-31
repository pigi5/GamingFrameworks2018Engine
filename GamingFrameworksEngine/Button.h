#pragma once
#include <SFML/Window.hpp>
#include "Command.h"

using namespace std;

// A "input" class that keeps track of what command it maps to
// as well as its state and the key that activates the command
class Button {
private:
	GameCommand command;
	sf::Keyboard::Key key;
	bool state = false;

public:
	Button(GameCommand command, sf::Keyboard::Key key);
	~Button();

	void setCommand(GameCommand command);
	void setKey(sf::Keyboard::Key key);

	void setState(bool state);
	bool isPressed();

	void updateState();

};