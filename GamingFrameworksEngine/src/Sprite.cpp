#include "../header/Sprite.h"

Sprite::Sprite(string filename, int xPos, int yPos, int xSize, int ySize)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->xSize = xSize;
	this->ySize = ySize;
	this->t = sf::Texture();
	this->r = sf::IntRect(sf::Vector2i(xPos, yPos), sf::Vector2i(xSize, ySize));
	t.loadFromFile(filename, r);
	this->s = sf::Sprite(t, r);
}

Sprite::~Sprite()
{
}

Rectangle* Sprite::getHitbox() const
{
    return hitbox;
}

void Sprite::setPosition(int xPos, int yPos) {
	this->xPos = xPos;
	this->yPos = yPos;
	s.setPosition(xPos, yPos);
}
void Sprite::move(int dx, int dy) {
	this->xPos += dx;
	this->yPos += dy;
	s.move(dx, dy);
}
void Sprite::rotate(int angle) {
	s.rotate(angle);
}
void Sprite::setSize(int xSize, int ySize) {
	this->xSize = xSize;
	this->ySize = ySize;
	s.setScale(xSize, ySize);
}
void Sprite::setTexture(string filename, int xSize, int ySize) {
	this->xSize = xSize;
	this->ySize = ySize;
	sf::Texture t = sf::Texture();
	sf::IntRect r = sf::IntRect(sf::Vector2i(xPos, yPos), sf::Vector2i(xSize, ySize));
	t.loadFromFile(filename, r);
	s.setTexture(t);
	s.setTextureRect(r);
}

void Sprite::draw(sf::RenderWindow* window) {
	window->draw(s);
}