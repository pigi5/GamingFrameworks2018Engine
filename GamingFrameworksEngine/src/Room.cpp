#include "../header/Room.h"

Room::Room()
{
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

void Room::draw()
{
    for (Actor* actor : actors)
    {
        actor->draw();
    }
}

void Room::drawHUD()
{
    for (Overlay* overlay : overlays)
    {
        overlay->draw();
    }
}