#pragma once

// Macro for cleaner trigger definitions
// Note that name is NOT in quotes
// type is the type of the id pointer in TriggerType
#define CREATE_TRIGGER(_name, type)             \
class _name : public TriggerType<type>          \
{                                               \
public:                                         \
    _name(const type* id) : TriggerType(id){};  \
    _name(const YAML::Node& node) : TriggerType(new type(node)) {}; \
};                                              \

#include "Trigger.h"
#include "Actor.h"
#include "yaml-cpp/yaml.h"
#include "Configurable.h"

struct ButtonInputType 
{
	short id;
	bool state = false;

	bool operator<(const ButtonInputType& other) const
	{
		return id * 2 + state < other.id * 2 + other.state;
	}

    ButtonInputType() {}

    ButtonInputType(const YAML::Node& config)
    {
        id = config["id"].as<short>();
        state = parseState(config["state"].as<std::string>());
    }
    
    static bool parseState(std::string state)
    {
        if (state != "up" && state != "down")
        {
            throw ConfigurationError("Button Input Type state must be \"down\" or \"up\"");
        }
        return state == "up";
    }
};

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