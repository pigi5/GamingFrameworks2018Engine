#include "../header/Music.h"
#include "../header/ConfigurationError.h"

const std::string Music::DIR_NAME = "music";
std::unordered_map<std::string, Music*> Music::objectMap;

void Music::createMusic(std::string name, std::string fileName)
{
	Music* newMusic = new Music(name, fileName);

	//add audio to map
	if (!objectMap.emplace(newMusic->name, newMusic).second)
	{
		//if audio already exists, throw error
		std::stringstream errorMessage;
		errorMessage << "Music name \"" << newMusic->name << "\" is not unique.";
		throw ConfigurationError(errorMessage.str());
	}
}

Music::~Music()
{

}

bool Music::load()
{
	return music.openFromFile(fileName);
}

void Music::play()
{
    if (load())
    {
	    music.play();
    }
}

void Music::stop()
{
	music.stop();
}