#pragma once
#include "../header/Actor.h"

class Wall : public Actor {
private:
	std::list<Actor*>& actorList;
	int xSize = 60, ySize = 20;
public:
	Wall(std::list<Actor*>& actorList);
	void step();
	void draw(sf::RenderWindow* window);
	void onCollision(const Actor* other);
};