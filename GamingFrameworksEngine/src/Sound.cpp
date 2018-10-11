#include "../header/Sound.h"
#include "../header/ConfigurationError.h"
#include "../header/Utils.h"

const std::string Sound::DIR_NAME = "sounds";
std::unordered_map<std::string, Sound*> Sound::objectMap;

void Sound::createSound(std::string name, std::string fileName)
{
	Sound* newSound = new Sound(name, fileName);

	//add audio to map
	if (!objectMap.emplace(newSound->name, newSound).second)
	{
		//if audio already exists, throw error
		std::stringstream errorMessage;
		errorMessage << "Sound name \"" << newSound->name << "\" is not unique.";
		throw ConfigurationError(errorMessage.str());
	}
}

Sound::Sound(std::string name, std::string fileName) : Audio(name, fileName)
{
    if (!load())
    {
        throw ConfigurationError("Sound named " + name + " could not be loaded from " + fileName + ".");
    }
}

Sound::Sound(const YAML::Node& config, bool shallow) : Audio(config, shallow)
{
    if (!load())
    {
        throw ConfigurationError("Sound named " + name + " could not be loaded from " + fileName + ".");
    }
}

Sound::~Sound()
{

}

//loads sound from given filename
bool Sound::load()
{
	return buffer.loadFromFile(fileName);
}

//plays sound
void Sound::play()
{
	sound.setBuffer(buffer);
	sound.play();
}

void Sound::stop()
{
    // can't
}