#pragma once

#include <unordered_map>
#include <string>
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
    static const std::string DIR_NAME;
    static std::unordered_map<std::string, OverlayType*> objectMap;

    static void createOverlayType(std::string);

    OverlayType(std::string);
    OverlayType(const YAML::Node&, bool);

    bool operator<(const OverlayType&) const;
};