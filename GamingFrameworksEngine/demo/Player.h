#pragma once
#include "../header/Actor.h"

class Player : public Actor {
private:
	std::list<Actor*>& actorList;

public:
	Player(std::list<Actor*>& actorList);
	void step();
	void draw(sf::RenderWindow* window);
	void onCollision(const Actor* other);
};