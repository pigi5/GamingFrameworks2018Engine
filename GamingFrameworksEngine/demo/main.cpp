#include "../header/Engine.h"
#include "HUD.h"
#include "Player.h"
#include "Wall.h"
#include <SFML\Graphics\RenderWindow.hpp>

int main()
{
	sf::VideoMode defaultMode = sf::VideoMode(800,600);
	sf::RenderWindow* window = new sf::RenderWindow();
	
	window->create(defaultMode, "Game Window", sf::Style::Titlebar | sf::Style::Close);

	std::list<Overlay*> overlays;
	HUD h = HUD();
	overlays.push_back(&h);
	std::list<Actor*> actors;
	actors.push_back(new Wall(actors));
	actors.push_back(new Player(actors));
	std::vector<Room*> rooms;
	rooms.push_back(new Room(actors, overlays));

	Engine engine = Engine(rooms);
	engine.run(window);

    return 0;
}