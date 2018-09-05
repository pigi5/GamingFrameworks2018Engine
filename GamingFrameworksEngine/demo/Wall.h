#pragma once
#include "../header/Actor.h"

class Wall : public Actor {
private:
	std::list<Actor*>& actorList;

public:
	Wall(std::list<Actor*>& actorList);
	void step();
	void draw(sf::RenderWindow* window);
	void onCollision(const Actor* other);
};