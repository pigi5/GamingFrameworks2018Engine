#pragma once
#include "../header/Actor.h"

class Player : public Actor {
private:
	std::list<Actor*>& actorList;
	int xSize = 30, ySize = 30;
public:
	Player(std::list<Actor*>& actorList);
	~Player();
	void step();
	void draw(sf::RenderWindow* window);
	void onCollision(const Actor* other);
};