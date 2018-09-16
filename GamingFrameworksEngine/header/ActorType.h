#pragma once

#include "../header/Trigger.h"
#include "../header/Action.h"
#include "../header/Material.h"
#include <map>
#include <list>

class ActorType
{
private:

public:
    // triggers/actions
    std::map<const Trigger*, std::list<const Action*>> actionMap;

    Material* material;

    ActorType(std::string);
};