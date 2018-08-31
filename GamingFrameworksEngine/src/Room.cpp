#include "../header/Room.h"

Room::Room()
{
}

Room::~Room()
{
}

void Room::step(double deltaTime)
{
    for (Actor* actor : actors)
    {
        actor->step(deltaTime);
    }

    for (Actor* actor : actors)
    {
        actor->move(deltaTime, actors);
    }
}

void Room::draw(double deltaTime)
{
    for (Actor* actor : actors)
    {
        actor->draw(deltaTime);
    }
}

void Room::drawHUD(double deltaTime)
{
    for (Overlay* overlay : overlays)
    {
        overlay->draw(deltaTime);
    }
}