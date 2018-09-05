#pragma once
#include <SFML/Window.hpp>

using namespace std;

// A "input" class that keeps track of what command it maps to
// as well as its state and the key that activates the command
class Button {
private:
	vector<double> params;
	sf::Keyboard::Key key;
	bool state = false;

public:
	Button();
	Button(sf::Keyboard::Key key, vector<double> params);
	~Button();

	void setKey(sf::Keyboard::Key key);
	void setParams(vector<double> params);
	vector<double> getParams();
	sf::Keyboard::Key getKey();

	void setState(bool state);
	bool isPressed();

	void updateState();

};