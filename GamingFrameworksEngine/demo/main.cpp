#include "../header/Engine.h"
#include "HUD.h"
#include "Player.h"
#include "Wall.h"
#include <SFML\Graphics\RenderWindow.hpp>

int main()
{
	sf::VideoMode defaultMode = sf::VideoMode(800,600);
	sf::RenderWindow window;
	
	window.create(defaultMode, "Game Window", sf::Style::Titlebar | sf::Style::Close);

	std::list<Actor*> overlays;
    HUD h({20, 20});
	overlays.push_back(&h);
	std::list<Actor*> actors;
    Wall wall({200, 400});
    Player player({200, 200});
	actors.push_back(&wall);
	actors.push_back(&player);
	std::vector<Room*> rooms;
    Room startRoom(actors, overlays);
	rooms.push_back(&startRoom);

	Engine engine = Engine(rooms);
	engine.run(&window);


    return 0;
}