#include <SFML/Graphics.hpp>
#include <iostream>
#include "Command.h"
#include "Button.h"

//KEYBOARD INPUT MAIN
int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		Button b1(Player1MoveUp, sf::Keyboard::Left);
		Button b2(Player1MoveUp, sf::Keyboard::Up);
		while (window.pollEvent(event))
		{
			b1.updateState();
			b2.updateState();
			if (event.type == sf::Event::Closed)
				window.close();

			if (b1.isPressed() && b2.isPressed())
			{
				// left and up key pressed, do a thing.
				std::cout << "left & up key pressed!" << std::endl;
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}