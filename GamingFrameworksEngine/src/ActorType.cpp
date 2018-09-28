#include <list>
#include "../header/ActorType.h"
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

std::map<const std::string, ActorType*> ActorType::objectMap;

ActorType::ActorType()
{
    name = "unknown";
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