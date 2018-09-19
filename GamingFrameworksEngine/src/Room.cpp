#include "../header/Room.h"

Room::Room(std::list<Actor*> actors, std::list<Actor*> overlays, string musicFile)
{
	this->actors = actors;
	this->overlays = overlays;
	this->music = Music(musicFile);
	this->music.playMusic();
}

Room::~Room()
{
	this->music.stopMusic();
}

void Room::step()
{
    for (Actor* actor : actors)
    {
        actor->step();
    }
    for (Actor* overlay : overlays)
    {
        overlay->step();
    }

    for (Actor* actor : actors)
    {
        actor->move(actors);
    }
    for (Actor* overlay : overlays)
    {
        overlay->move(overlays);
    }
}

void Room::interpolateState(float progress)
{
	for (Actor* actor : actors)
	{
		actor->interpolateState(progress);
	}
    for (Actor* overlay : overlays)
    {
        overlay->interpolateState(progress);
    }
}

void Room::draw(sf::RenderWindow* window, sf::View* view)
{
    for (Actor* actor : actors)
    {
        actor->draw(window, view);
    }
}

void Room::drawHUD(sf::RenderWindow* window, sf::View* view)
{
    for (Actor* overlay : overlays)
    {
        overlay->draw(window, view);
    }
}