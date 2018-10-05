#include "../header/Audio.h"

std::map<const std::string, Audio*> Audio::audioMap;

//basic constructor
Audio::Audio()
{
	fileName = "";
}

//constructor that deserializes a YAML node
Audio::Audio(const YAML::Node& config, bool shallow)
{
	fileName = config["file_name"].as<std::string>();

	YAML::Node triggers = config["triggers"];

	if (!shallow)
	{
		for (YAML::Node trigger : triggers)
		{
			Trigger* newTrigger = trigger_preset::createTrigger(trigger["type"].as<std::string>(), trigger);
			std::list<Action*> actionList;

			if (trigger["actions"])
			{
				YAML::Node actions = trigger["actions"];

				for (YAML::Node action : actions)
				{
					actionList.push_back(action_preset::createAction(action["type"].as<std::string>(), action));
				}
			}
			actionMap[newTrigger] = actionList;
		}
	}
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Audio& obj)
{
	out << YAML::Key << "file_name" << YAML::Value << obj.fileName;
	out << YAML::Key << "triggers" << YAML::Value << YAML::BeginSeq;
	for (auto pair : obj.actionMap)
	{
		out << *(pair.first);
		out << YAML::Key << "actions" << YAML::Value << YAML::BeginSeq;
		for (Action* action : pair.second)
		{
			out << *action;
		}
		out << YAML::EndSeq;
	}
	out << YAML::EndSeq;
	return out;
}

//constructor that takes the name of an audio file
Audio::Audio(std::string fileName) {
	this->fileName = fileName;
}

//destructor
Audio::~Audio() 
{
}