#include <list>
#include "../header/OverlayType.h"
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

std::map<const std::string, OverlayType*> OverlayType::objectMap;

OverlayType::OverlayType() : ActorType()
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