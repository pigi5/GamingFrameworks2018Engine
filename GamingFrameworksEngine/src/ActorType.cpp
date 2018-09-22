#include <list>
#include "../header/ActorType.h"
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

std::map<const std::string, ActorType*> ActorType::objectMap;

ActorType::ActorType(const YAML::Node& config)
{
    name = config["name"].as<std::string>();

    if (config["material"])
    {
        YAML::Node materialNode = config["material"];
        if (!materialNode.IsNull())
        {
            material = Material::objectMap[materialNode.as<std::string>()];
        }
    }

    if (config["maxXSpeed"])
    {
        maxXSpeed = config["maxXSpeed"].as<float>();
    }
    else
    {
        maxXSpeed = -1;
    }

    if (config["maxYSpeed"])
    {
        maxYSpeed = config["maxYSpeed"].as<float>();
    }
    else
    {
        maxYSpeed = -1;
    }

    if (config["gravitous"])
    {
        gravitous = config["gravitous"].as<bool>();
    }
    else
    {
        gravitous = true;
    }
    
    if (config["triggers"])
    {
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

std::ostream& operator<<(std::ostream& output, const ActorType& object)
{
    output << " name: " << object.name;
    if (object.material == NULL)
    {
        output << " material: NULL";
    }
    else
    {
         output << " material: " << object.material;
    }
   
    for (const auto& pair : object.actionMap)
    {
        output << " trigger: ";
        for (const Action* action : pair.second)
        {
            output << " action ";
        }
    }
    return output;
}