#include <list>
#include "../header/ActorType.h"
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

const std::string ActorType::DIR_NAME = "actor_types";
std::unordered_map<std::string, ActorType*> ActorType::objectMap;

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
    friction = 0.0f;
    maxXSpeed = -1.0f;
    maxYSpeed = -1.0f;
    gravitous = true;
    collidable = true;
    xScale = 1.0f;
    yScale = 1.0f;
    imageSpeed = 0.0f;
    depth = 0.0f;
}

ActorType::ActorType(const YAML::Node& config, bool shallow)
{
    name = config["name"].as<std::string>();

    if (!shallow)
    {
        friction = config["friction"].as<float>();
        maxXSpeed = config["maxXSpeed"].as<float>();
        maxYSpeed = config["maxYSpeed"].as<float>();
        gravitous = config["gravitous"].as<bool>();
        collidable = config["collidable"].as<bool>();

		YAML::Node spriteNode = config["sprite"];
		if (!spriteNode.IsNull()) {
			string spriteName = spriteNode.as<std::string>();
			auto checkName = Sprite::objectMap.find(spriteName);
			if (checkName == Sprite::objectMap.end())
			{
				std::stringstream errorMessage;
				errorMessage << "Sprite " << spriteName << " does not exist.";
				throw ConfigurationError(errorMessage.str());
			}
			sprite = checkName->second;
		}

        xScale = config["xScale"].as<float>();
        yScale = config["yScale"].as<float>();
        imageSpeed = config["animationSpeed"].as<float>();
        depth = config["depth"].as<float>();
        
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

YAML::Emitter& operator<<(YAML::Emitter& out, const ActorType& obj) 
{
    out << YAML::Key << "name" << YAML::Value << obj.name;
    out << YAML::Key << "friction" << YAML::Value << obj.friction;
    out << YAML::Key << "maxXSpeed" << YAML::Value << obj.maxXSpeed;
    out << YAML::Key << "maxYSpeed" << YAML::Value << obj.maxYSpeed;
    out << YAML::Key << "gravitous" << YAML::Value << obj.gravitous;
    out << YAML::Key << "collidable" << YAML::Value << obj.collidable;
    if (obj.sprite == NULL) 
    {
        out << YAML::Key << "sprite" << YAML::Value << YAML::Null;
    }
    else 
    {
        out << YAML::Key << "sprite" << YAML::Value << obj.sprite->name;
    }
    out << YAML::Key << "xScale" << YAML::Value << obj.xScale;
    out << YAML::Key << "yScale" << YAML::Value << obj.yScale;
    out << YAML::Key << "animationSpeed" << YAML::Value << obj.imageSpeed;
    out << YAML::Key << "depth" << YAML::Value << obj.depth;
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
            out << YAML::BeginMap;
            out << *action;
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    return out;
}

Logger& operator<<(Logger& logger, const ActorType& obj) 
{
    logger << obj.name;
    return logger;
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

size_t ActorType::hashCode() const
{
    static hash<std::string> hasher;
    return hasher(name);
}

const std::string ActorType::toString() const
{
    return name;
}