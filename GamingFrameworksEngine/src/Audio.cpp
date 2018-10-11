#include "../header/Audio.h"
#include "../header/ConfigurationError.h"


const std::string Audio::DIR_NAME = "audio";
std::unordered_map<std::string, Audio*> Audio::objectMap;

void Audio::createAudio(std::string name)
{
	Audio* newAudio = new Audio(name);

	//add audio to map
	if (!objectMap.emplace(newAudio->name, newAudio).second)
	{
		//if audio already exists, throw error
		std::stringstream errorMessage;
		errorMessage << "Audio name \"" << newAudio->name << "\" is not unique.";
		throw ConfigurationError(errorMessage.str());
	}
}


//basic constructor
Audio::Audio()
{
	name = "";
	fileName = "";
	audioType = "";
}

//constructor that deserializes a YAML node
Audio::Audio(const YAML::Node& config, bool shallow)
{
	name = config["name"].as<std::string>();
	fileName = config["file_name"].as<std::string>();
	audioType = config["audio_type"].as<std::string>();
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Audio& obj)
{
	out << YAML::Key << "name" << YAML::Value << obj.name;
	out << YAML::Key << "file_name" << YAML::Value << obj.fileName;
	out << YAML::Key << "audio_type" << YAML::Value << obj.audioType;
	return out;
}

bool Audio::operator<(const Audio& other) const
{
	return this->fileName < other.fileName;
}

//constructor that takes the name of an audio file
Audio::Audio(std::string name)
{
	this->name = name;
	this->fileName = "";
	this->audioType = "";
}

//destructor
Audio::~Audio() 
{
}

Logger& operator<<(Logger& logger, const Audio& obj)
{
	logger << obj.name;
	return logger;
}