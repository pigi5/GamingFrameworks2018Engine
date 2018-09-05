#pragma once
#include "../header/Actor.h"
#include "../header/Command.h"
#include "../header/Button.h"
#include <vector>

class Player : public Actor {
private:
	std::list<Actor*>& actorList;
	int xSize = 30, ySize = 30;
	vector<Button> buttons;
public:
	Player(std::list<Actor*>& actorList);
	~Player();
	void step();
	void draw(sf::RenderWindow* window, sf::View* view);
	void onCollision(const Actor* other);
};