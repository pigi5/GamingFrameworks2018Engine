#include "../header/Audio.h"

Audio::Audio(std::string name, std::string fileName)
{
    this->name = name;
    this->fileName = fileName;
}

//constructor that deserializes a YAML node
Audio::Audio(const YAML::Node& config, bool shallow)
{
	name = config["name"].as<std::string>();
	fileName = config["file_name"].as<std::string>();
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Audio& obj)
{
	out << YAML::Key << "name" << YAML::Value << obj.name;
	out << YAML::Key << "file_name" << YAML::Value << obj.fileName;
	return out;
}

bool Audio::operator<(const Audio& other) const
{
	return this->fileName < other.fileName;
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