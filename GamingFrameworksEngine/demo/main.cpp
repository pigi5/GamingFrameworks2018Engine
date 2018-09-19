#include "../header/Engine.h"
#include "HUD.h"
#include "Player.h"
#include "Wall.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <iostream>
#include <direct.h> 

int main()
{
    char a;
    // print current working directory
    char buffer[256];
    char *val = _getcwd(buffer, sizeof(buffer));
    if (val) {
        std::cout << "Running from: " << buffer << std::endl;
    }

    loadAll<Material>("./resources/materials");
    loadAll<ActorType>("./resources/actor_types");

	sf::VideoMode defaultMode = sf::VideoMode(800,600);
	sf::RenderWindow window;
	
	window.create(defaultMode, "Game Window", sf::Style::Titlebar | sf::Style::Close);

	std::list<Actor*> overlays;
    HUD h({20, 20});
	overlays.push_back(&h);
	std::list<Actor*> actors;
    // example: Actor player(ActorType::objectMap["player"]);
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