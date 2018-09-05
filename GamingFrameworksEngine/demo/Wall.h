#pragma once
#include "../header/Actor.h"

class Wall : public Actor {
private:
	std::list<Actor*>& actorList;
	int xSize = 200, ySize = 20;
public:
	Wall(std::list<Actor*>& actorList);
	~Wall();
	void step();
	void draw(sf::RenderWindow* window, sf::View* view);
	void onCollision(const Actor* other);
};