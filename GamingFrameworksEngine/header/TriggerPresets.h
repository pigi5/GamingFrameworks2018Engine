#pragma once

// Macro for cleaner trigger definitions
// Note that name is NOT in quotes
// type is the type of the id pointer in TriggerType
#define CREATE_TRIGGER(_name, type)             \
class _name : public TriggerType<type>          \
{                                               \
public:                                         \
    _name(type* id) : TriggerType(id){};        \
    _name(const YAML::Node& node) : TriggerType(new type(node)) {}; \
};                                              \

#include "Trigger.h"
#include "Actor.h"
#include "../header/InputHandler.h"
#include "yaml-cpp/yaml.h"
#include "Configurable.h"


// All trigger presets are defined here
namespace trigger_preset
{
    CREATE_TRIGGER(Collision, ActorType);
    CREATE_TRIGGER(ButtonInput, ButtonInputType);
    CREATE_TRIGGER(Create, ActorType);
    CREATE_TRIGGER(Step, ActorType);
    CREATE_TRIGGER(Draw, ActorType);
    CREATE_TRIGGER(Destroy, ActorType);

    // abstract factory
    static Trigger* createTrigger(const std::string typeName, const YAML::Node& node)
    {
        if (typeName == "Collision")
        {
            return new Collision(node);
        }
        else if (typeName == "ButtonInput")
        {
            return new ButtonInput(node);
        }
        else if (typeName == "Create")
        {
            return new Create(node);
        }
        else if (typeName == "Step")
        {
            return new Step(node);
        }
        else if (typeName == "Draw")
        {
            return new Draw(node);
        }
        else if (typeName == "Destroy")
        {
            return new Destroy(node);
        }
        else
        {
            std::stringstream errorMessage;
            errorMessage << "Trigger " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
    }
}

#undef CREATE_TRIGGER