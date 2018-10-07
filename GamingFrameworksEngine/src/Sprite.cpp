#include <cmath>
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

Sprite::Sprite(std::string name) 
{
	this->name = name;
}

Sprite::Sprite(const YAML::Node& config, bool shallow)
{
	name = config["name"].as<std::string>();
	if (!shallow) {
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
}

float Sprite::getRecommendedWidth()
{
    return textures[0]->getSize().x;
}

float Sprite::getRecommendedHeight()
{
    return textures[0]->getSize().y;
}

void Sprite::draw(sf::RenderWindow* window, float index, float xPos, float yPos, float xScale, float yScale, float angle) 
{
    s.setTexture(*textures[fmod(index, textures.size())]);
    s.setPosition(xPos, yPos);
	s.setScale(xScale, yScale);
	s.setRotation(angle);
	window->draw(s);
}

YAML::Emitter & operator<<(YAML::Emitter & out, const Sprite & obj)
{
	out << YAML::Key << "name" << YAML::Value << obj.name;
	out << YAML::Key << "textures" << YAML::Value << YAML::BeginSeq;
	for (auto s : obj.textrFiles) {
		out << s;
	}
	out << YAML::EndSeq;
	return out;
}
