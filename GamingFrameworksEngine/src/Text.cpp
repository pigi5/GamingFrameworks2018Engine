#include "../header/Text.h"

Text::Text(string txt, int xPos, int yPos) {
	this->str = txt;
	this->xPos = xPos;
	this->yPos = yPos;
	this->size = 32;
	this->color = sf::Color();
	this->style = 'r';
	this->font.loadFromFile("./demo/ariblk.ttf");
	t = sf::Text(txt, font, size);
	t.setPosition(xPos, yPos);
}
void Text::setText(string txt) {
	this->str = txt;
	t.setString(txt);
}
void Text::setPosition(int xPos, int yPos) {
	this->xPos = xPos;
	this->yPos = yPos;
	t.setPosition(xPos, yPos);
}
void Text::move(int dx, int dy) {
	this->xPos += dx;
	this->yPos += dy;
	t.move(dx, dy);
}
void Text::rotate(int angle) {
	t.rotate(angle);
}
void Text::setSize(int size) {
	this->size = size;
	t.setCharacterSize(size);
}
void Text::setFont(sf::Font font) {
	this->font = font;
	t.setFont(font);
}
void Text::setStyle(char style) {
	this->style = style;
	switch (style)
	{
	case 'r': t.setStyle(sf::Text::Regular);
		break;
	case 'b': t.setStyle(sf::Text::Bold);
		break;
	case 'i': t.setStyle(sf::Text::Italic);
		break;
	case 's': t.setStyle(sf::Text::StrikeThrough);
		break;
	}
}
void Text::setColor(sf::Color color) {
	this->color = color;
	t.setFillColor(color);
}

void Text::draw(sf::RenderWindow* window) {
	window->draw(t);
}