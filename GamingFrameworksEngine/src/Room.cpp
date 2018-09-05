#include "../header/Room.h"

Room::Room(std::list<Actor*> actors, std::list<Overlay*> overlays)
{
	this->actors = actors;
	this->overlays = overlays;
}

Room::~Room()
{
}

void Room::step()
{
    for (Actor* actor : actors)
    {
        actor->step();
    }

    for (Actor* actor : actors)
    {
        actor->move(actors);
    }
}

void Room::interpolateState(float progress)
{
	for (Actor* actor : actors)
	{
		actor->interpolateState(progress);
	}
}

void Room::draw(sf::RenderWindow* window)
{
    for (Actor* actor : actors)
    {
        actor->draw(window);
    }
}

void Room::drawHUD(sf::RenderWindow* window)
{
    for (Overlay* overlay : overlays)
    {
        overlay->draw(window);
    }
}