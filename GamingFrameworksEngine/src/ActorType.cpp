#include <list>
#include "../header/ActorType.h"
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

const std::string ActorType::DIR_NAME = "actor_types";

std::map<const std::string, ActorType*> ActorType::objectMap;

void ActorType::createActorType(std::string name)
{
    ActorType* newType = new ActorType(name);
    
    // add actor type to map, keyed by name
    if (!objectMap.emplace(newType->name, newType).second)
    {
        // if key already existed, throw error
        std::stringstream errorMessage;
        errorMessage << "Actor Type name \"" << newType->name << "\" is not unique.";
        throw ConfigurationError(errorMessage.str());
    }
}

ActorType::ActorType(std::string name)
{
    this->name = name;
    material = NULL;
    maxXSpeed = -1;
    maxYSpeed = -1;
    gravitous = true;
}

ActorType::ActorType(const YAML::Node& config, bool shallow)
{
    name = config["name"].as<std::string>();

    if (!shallow)
    {
        YAML::Node materialNode = config["material"];
        if (!materialNode.IsNull())
        {
            std::string typeName = materialNode.as<std::string>();
            auto mapItem = Material::objectMap.find(typeName);
            if (mapItem == Material::objectMap.end())
            {
                std::stringstream errorMessage;
                errorMessage << "Material Type " << typeName << " does not exist.";
                throw ConfigurationError(errorMessage.str());
            }
            material = mapItem->second;
        }

        maxXSpeed = config["maxXSpeed"].as<float>();
        maxYSpeed = config["maxYSpeed"].as<float>();
        gravitous = config["gravitous"].as<bool>();

		YAML::Node spriteNode = config["spriteName"];
		if (!spriteNode.IsNull()) {
			spriteName = spriteNode.as<std::string>();
			auto checkName = Sprite::objectMap.find(spriteName);
			if (checkName == Sprite::objectMap.end())
			{
				std::stringstream errorMessage;
				errorMessage << "Sprite " << spriteName << " does not exist.";
				throw ConfigurationError(errorMessage.str());
			}
		}
        
        YAML::Node attrsNode = config["attributes"];
        for (auto attribute : attrsNode)
        {
            std::string attrName = attribute.first.as<std::string>();

            if (!attributes.emplace(attrName, attribute.second.as<int>()).second)
            {
                // if key already existed, throw error
                std::stringstream errorMessage;
                errorMessage << "Attribute name \"" << attrName;
                errorMessage << "\" (referenced in " << name << ") is not unique.";
                throw ConfigurationError(errorMessage.str());
            }
        }
    
        YAML::Node triggers = config["triggers"];

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
    
// TODO multiple of these i think
YAML::Emitter& operator<<(YAML::Emitter& out, const ActorType& obj) 
{
    out << YAML::Key << "name" << YAML::Value << obj.name;
    out << YAML::Key << "material" << YAML::Value << obj.material->name;
    out << YAML::Key << "maxXSpeed" << YAML::Value << obj.maxXSpeed;
    out << YAML::Key << "gravitous" << YAML::Value << obj.gravitous;
    out << YAML::Key << "attributes" << YAML::Value << YAML::BeginMap;
    for (auto pair : obj.attributes)
    {
        out << YAML::Key << pair.first << YAML::Value << pair.second;
    }
    out << YAML::EndMap;
    out << YAML::Key << "triggers" << YAML::Value << YAML::BeginSeq;
    for (auto pair : obj.actionMap)
    {
        out << YAML::BeginMap;
        out << *(pair.first);
        out << YAML::Key << "actions" << YAML::Value << YAML::BeginSeq;
        for (Action* action : pair.second)
        {
            out << *action;
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    return out;
}

ActorType::~ActorType()
{
    for (const auto& pair : actionMap)
    {
        delete pair.first;
        for (const Action* action : pair.second)
        {
            delete action;
        }
    }
}

bool ActorType::operator<(const ActorType& other) const
{
    return name < other.name;
}