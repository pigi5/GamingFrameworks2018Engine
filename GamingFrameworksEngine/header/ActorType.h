#pragma once

#include <map>
#include <list>
#include <iostream>
#include "../header/Configurable.h"
#include "../header/Trigger.h"
#include "../header/Action.h"
#include "../header/Material.h"
#include "yaml-cpp/yaml.h"

class ActorType
{
private:

public:
    static std::map<const std::string, const ActorType*> objectMap;
    
    std::string name;
    const Material* material;

    // triggers/actions
    std::map<const Trigger*, std::list<const Action*>> actionMap;
    ActorType(const YAML::Node&);
    ~ActorType();
    bool operator<(const ActorType& other) const;
    friend std::ostream& operator<<(std::ostream&, const ActorType&);
};