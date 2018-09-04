#pragma once
#include <SFML\Graphics.hpp>

using namespace std;

class Text {
private:
	sf::Text t;
	string str;
	sf::Font font;
	int size;
	int xPos, yPos;
	char style;
	sf::Color color;

public:
	Text(string txt, int xPos, int yPos);
	void setText(string txt);
	void setPosition(int xPos, int yPos);
	void move(int dx, int dy);
	void rotate(int angle);
	void setSize(int size);
	void setFont(sf::Font font);
	void setStyle(char style);
	void setColor(sf::Color color);
	void draw(sf::RenderWindow* window);
};