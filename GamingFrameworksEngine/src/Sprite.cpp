#include "../header/Sprite.h"
#include "../header/ConfigurationError.h"

const std::string Sprite::DIR_NAME = "sprites";
std::map<const std::string, Sprite*> Sprite::objectMap;

void Sprite::createSprite(std::string name)
{
	
	Sprite* newType = new Sprite(name);

	// add sprite to map, keyed by name
	if (!objectMap.emplace(newType->name, newType).second)
	{
		// if key already existed, throw error
		std::stringstream errorMessage;
		errorMessage << "Sprite name \"" << newType->name << "\" is not unique.";
		throw ConfigurationError(errorMessage.str());
	}
}

Sprite::Sprite()
{
	this->xPos = 0;
	this->yPos = 0;
	this->name = "";
	this->numFrames = 1;
}

Sprite::Sprite(std::string name) {
	this->xPos = 0;
	this->yPos = 0;
	this->name = "";
	this->numFrames = 1;
}

Sprite::Sprite(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->name = "";
	this->numFrames = 1;
}

Sprite::Sprite(const YAML::Node& config, bool shallow)
{
	this->xPos = 0;
	this->yPos = 0;
	name = config["name"].as<std::string>();
	if (!shallow) {
		numFrames = config["numFrames"].as<int>();
		xSize = config["xSize"].as<int>();
		ySize = config["ySize"].as<int>();

		YAML::Node txtrNode = config["textures"];
		sf::Texture* t;
		for (auto texture : txtrNode)
		{
			std::string filename = texture.as<std::string>();
			textrFiles.push_back(filename);
			t = new sf::Texture();
			t->loadFromFile(filename);
			textures.push_back(t);
		}
	}

}

Sprite::~Sprite()
{
}

Hitbox* Sprite::getHitbox() const
{
    return hitbox;
}

void Sprite::changeTexture(int pos, string filename)
{
	if (pos < textures.size()) {
		sf::Texture* t;
		t = new sf::Texture();
		t->loadFromFile(filename);
		textures.push_back(t);
	}
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
void Sprite::setName(string name)
{
	this->name = name;
}
void Sprite::setSize(int xSize, int ySize) {
	this->xSize = xSize;
	this->ySize = ySize;
	s.setScale(xSize, ySize);
}

void Sprite::setSprite(int pos)
{
	if (pos < textures.size())
	{
		s.setTexture(*textures[pos]);
	}
}

void Sprite::addTexture(string filename) {
	sf::Texture* t;
	t = new sf::Texture();
	t->loadFromFile(filename);
	textures.push_back(t);
}

void Sprite::setPartialTexture(string filename, int xSize, int ySize) {
	sf::IntRect r;
	sf::Texture* t;
	t = new sf::Texture();
	r = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(xSize, ySize));
	t->loadFromFile(filename, r);
	textures.push_back(t);
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
		sf::IntRect rect = sf::IntRect(sf::Vector2i((j * xSize), (k * ySize)), sf::Vector2i((j+1) * xSize, (k+1) * ySize));
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

YAML::Emitter & operator<<(YAML::Emitter & out, const Sprite & obj)
{
	out << YAML::Key << "name" << YAML::Value << obj.name;
	out << YAML::Key << "numFrames" << YAML::Value << obj.numFrames;
	out << YAML::Key << "xSize" << YAML::Value << obj.xSize;
	out << YAML::Key << "ySize" << YAML::Value << obj.ySize;
	out << YAML::Key << "textures" << YAML::Value << YAML::BeginSeq;
	for (auto s : obj.textrFiles) {
		out << s;
	}
	out << YAML::EndSeq;
}
