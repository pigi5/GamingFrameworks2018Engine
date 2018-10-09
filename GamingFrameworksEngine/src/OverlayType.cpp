#include <list>
#include "../header/OverlayType.h"
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

const std::string OverlayType::DIR_NAME = "overlay_types";
std::unordered_map<std::string, OverlayType*> OverlayType::objectMap;

void OverlayType::createOverlayType(std::string name)
{
    OverlayType* newType = new OverlayType(name);
    
    // add overlay type to map, keyed by name
    if (!objectMap.emplace(newType->name, newType).second)
    {
        // if key already existed, throw error
        std::stringstream errorMessage;
        errorMessage << "Overlay Type name \"" << newType->name << "\" is not unique.";
        throw ConfigurationError(errorMessage.str());
    }
}

OverlayType::OverlayType(std::string name) : ActorType(name)
{
    gravitous = false;
}

OverlayType::OverlayType(const YAML::Node& config, bool shallow) : ActorType(config, shallow) 
{
    if (config["gravitous"])
    {
        gravitous = config["gravitous"].as<bool>();
    }
    else
    {
        gravitous = false;
    }
}
bool OverlayType::operator<(const OverlayType& other) const
{
    return name < other.name;
}