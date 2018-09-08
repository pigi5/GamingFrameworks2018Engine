#pragma once
#include "../header/Actor.h"
#include "../header/Command.h"
#include "../header/Button.h"
#include <vector>

class Player : public Actor {
private:
    State startState;
	int xSize;
    int ySize;
	vector<Button> buttons;
public:
	Player(State startState);
	void step();
	void draw(sf::RenderWindow* window, sf::View* view);
};