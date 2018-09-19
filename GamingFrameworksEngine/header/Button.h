#pragma once
#include <SFML/Window.hpp>
#include "Sound.h"
#include <map>

using namespace std;

// A "input" class that keeps track of what command it maps to
// as well as its state and the key that activates the command
class Button {
private:
	vector<double> params; // will probably scrap this
	sf::Keyboard::Key key;
	Sound sound;
	bool state = false;
	bool hasSound = false;

public:
	Button();
	Button(sf::Keyboard::Key key, vector<double> params);
	Button(sf::Keyboard::Key key, vector<double> params, string soundFile);
	~Button();

	void setKey(sf::Keyboard::Key key);
	void setParams(vector<double> params);
	vector<double> getParams();
	sf::Keyboard::Key getKey();

	void setState(bool state);
	bool isPressed();

	void updateState();

};