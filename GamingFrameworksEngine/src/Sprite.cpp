#include "../header/Sprite.h"

Sprite::Sprite(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
}

Sprite::~Sprite()
{
}

Hitbox* Sprite::getHitbox() const
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

void Sprite::setTexture(string filename) {
	this->t = sf::Texture();
	t.loadFromFile(filename);
	s.setTexture(t);
	this->xSize = t.getSize().x;
	this->ySize = t.getSize().y;
}

void Sprite::setPartialTexture(string filename, int xSize, int ySize) {
	this->xSize = xSize;
	this->ySize = ySize;
	sf::Texture t = sf::Texture();
	sf::IntRect r = sf::IntRect(sf::Vector2i(xPos, yPos), sf::Vector2i(xSize, ySize));
	t.loadFromFile(filename, r);
	s.setTexture(t);
	s.setTextureRect(r);
}

vector<sf::Sprite> Sprite::parseSpriteSheet(string filename, int xSize, int ySize, int num) {
	vector<sf::Sprite> vect;
	sf::Sprite spr = sf::Sprite();
	sf::Texture tex = sf::Texture();
	tex.loadFromFile(filename);
	this->xSize = tex.getSize().x;
	this->ySize = tex.getSize().y;
	int j = 0, k = 0;
	for (int i = 0; i < num; i++) {
		sf::IntRect rect = sf::IntRect(sf::Vector2i((j * xSize), (k * ySize)), sf::Vector2i(xSize, ySize));
		tex.loadFromFile(filename, rect);
		spr.setTexture(tex);
		spr.setTextureRect(rect);
		vect.push_back(spr);
		if (j * xSize >= this->xSize) {
			k++;
			j = 0;
		}
		else {
			j++;
		}
	}
	this->xSize = xSize;
	this->ySize = ySize;
}

void Sprite::draw(sf::RenderWindow* window) {
	window->draw(s);
}