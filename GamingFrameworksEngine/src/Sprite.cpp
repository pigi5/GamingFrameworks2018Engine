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
}

Sprite::Sprite(std::string name) {
	this->xPos = 0;
	this->yPos = 0;
	this->name = name;
}

Sprite::Sprite(int xPos, int yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->name = "";
}

Sprite::Sprite(const YAML::Node& config, bool shallow)
{
	this->xPos = 0;
	this->yPos = 0;
	name = config["name"].as<std::string>();
	if (!shallow) {
		xSize = config["xSize"].as<int>();
		ySize = config["ySize"].as<int>();

		YAML::Node txtrNode = config["textures"];
		sf::Texture* t;
		for (auto texture : txtrNode)
		{
			std::string filename = texture.as<std::string>();
			textrFiles.push_back(filename);
			t = new sf::Texture();
			bool loaded = t->loadFromFile(filename);
			if (!loaded)
			{
                std::stringstream errorMessage;
                errorMessage << "Texture " << filename << " could not be loaded.";
                throw ConfigurationError(errorMessage.str());
			}
			textures.push_back(t);
		}
		s.setTexture(*textures[0]);
	}

}

Sprite::~Sprite()
{
    for (sf::Texture* texture : textures)
    {
        delete texture;
    }
    textures.clear();
    if (hitbox != NULL)
    {
        delete hitbox;
    }
}

Sprite& Sprite::operator=(const Sprite& other)
{
    name = other.name;
    if (hitbox != NULL)
    {
        *hitbox = *other.hitbox;
    }
    textures = other.textures;
    textrFiles = other.textrFiles;
    xSize = other.xSize;
    ySize = other.ySize;
    xPos = other.xPos;
    yPos = other.yPos;
    return *this;
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
		bool loaded = t->loadFromFile(filename);
		if (!loaded)
		{
			// TODO handle this error
		}
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
	bool loaded = t->loadFromFile(filename);
	if (!loaded)
	{
		// TODO handle this error
	}
	textures.push_back(t);
}

void Sprite::setPartialTexture(string filename, int xSize, int ySize) {
	sf::IntRect r;
	sf::Texture* t;
	t = new sf::Texture();
	r = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(xSize, ySize));
	bool loaded = t->loadFromFile(filename, r);
	if (!loaded)
	{
		// TODO handle this error
	}
	textures.push_back(t);
}


void Sprite::draw(sf::RenderWindow* window) {
	window->draw(s);
}

YAML::Emitter & operator<<(YAML::Emitter & out, const Sprite & obj)
{
	out << YAML::Key << "name" << YAML::Value << obj.name;
	out << YAML::Key << "xSize" << YAML::Value << obj.xSize;
	out << YAML::Key << "ySize" << YAML::Value << obj.ySize;
	out << YAML::Key << "textures" << YAML::Value << YAML::BeginSeq;
	for (auto s : obj.textrFiles) {
		out << s;
	}
	out << YAML::EndSeq;
	return out;
}
