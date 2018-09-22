#pragma once

#include <map>
#include <list>
#include <iostream>
#include "../header/Configurable.h"
#include "../header/Trigger.h"
#include "../header/Action.h"
#include "../header/ActorType.h"
#include "../header/Material.h"
#include "yaml-cpp/yaml.h"

class OverlayType : public ActorType
{
private:

public:
    static std::map<const std::string, OverlayType*> objectMap;

    OverlayType(const YAML::Node& config) : ActorType(config) {};

    bool operator<(const OverlayType&) const;
    friend std::ostream& operator<<(std::ostream&, const OverlayType&);
};