#pragma once

// Macro for cleaner trigger definitions
// Note that name is NOT in quotes
// type is the type of the id pointer in TriggerType
#define CREATE_TRIGGER(_name, type)                                 \
class _name : public TriggerType<type>                              \
{                                                                   \
public:                                                             \
    _name(const type* id) : TriggerType(id){};                      \
    _name(const YAML::Node& node) : TriggerType(new type(node)) {}; \
    YAML::Emitter& serialize(YAML::Emitter& out) const              \
    {                                                               \
        return TriggerType::serialize(out, #_name);                 \
    }                                                               \
};                                                                  \

#include "Trigger.h"
#include "Actor.h"
#include "yaml-cpp/yaml.h"
#include "Configurable.h"
#include "../header/ButtonStates.h"

// ID structures //

struct ButtonInputType 
{
	short id;
	ButtonState state = ButtonState::RELEASE;

	bool operator<(const ButtonInputType& other) const
	{
		return id * 3 + state < other.id * 3 + other.state;
	}

    ButtonInputType() {}

    ButtonInputType(const YAML::Node& config)
    {
        id = config["id"].as<short>();
        state = ButtonInputType::parseState(config["state"].as<std::string>());
    }
    
    static ButtonState parseState(std::string state)
    {
		ButtonState bs;
        if (state != "up" && state != "down" && state != "hold")
        {
            throw ConfigurationError("Button Input Type state must be \"down\", \"up\" or \"hold\"");
        }
		if (state == "up")
			bs = ButtonState::RELEASE;
		else if (state == "down")
			bs = ButtonState::PRESS;
		else
			bs = ButtonState::HOLD;
        return bs;
    }
    
    static std::string emitState(ButtonState bs)
    {
		string state = "";
		switch (bs)
		{
		case PRESS:
			state = "down";
			break;
		case RELEASE:
			state = "up";
			break;
		case HOLD:
			state = "hold";
			break;
		default:
			break;
		}
        return state;
    }
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const ButtonInputType& obj)
    {
        out << YAML::Key << "id" << YAML::Value << obj.id;
        out << YAML::Key << "state" << YAML::Value << ButtonInputType::emitState(obj.state);
        return out;
    }
};


struct Index 
{
	int index;

	bool operator<(const Index& other) const
	{
		return index < other.index;
	}

    Index(int index) 
    {
        this->index = index;
    }

    Index(const YAML::Node& config)
    {
        index = config["index"].as<int>();
    }
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const Index& obj)
    {
        out << YAML::Key << "index" << YAML::Value << obj.index;
        return out;
    }
};


struct ActorTypeWrapper 
{
	const ActorType* type;

	bool operator<(const ActorTypeWrapper& other) const
	{
		return *type < *(other.type);
	}

    ActorTypeWrapper(const ActorType* type) 
    {
        this->type = type;
    }

    ActorTypeWrapper(const YAML::Node& config)
    {
        std::string typeName = config["target"].as<std::string>();
        auto mapItem = ActorType::objectMap.find(typeName);
        if (mapItem == ActorType::objectMap.end())
        {
            std::stringstream errorMessage;
            errorMessage << "Actor Type " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
        type = mapItem->second;
    }
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const ActorTypeWrapper& obj)
    {
        out << YAML::Key << "type" << YAML::Value << obj.type;
        return out;
    }
};

// All trigger presets are defined here
namespace trigger_preset
{
    CREATE_TRIGGER(Collision, ActorTypeWrapper)
    CREATE_TRIGGER(ButtonInput, ButtonInputType)
    CREATE_TRIGGER(Create, ActorTypeWrapper)
    CREATE_TRIGGER(Step, ActorTypeWrapper)
    CREATE_TRIGGER(Draw, ActorTypeWrapper)
    CREATE_TRIGGER(Destroy, ActorTypeWrapper)
    CREATE_TRIGGER(Timer, Index)
    CREATE_TRIGGER(Custom, Index)

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
        else if (typeName == "Timer")
        {
            return new Timer(node);
        }
        else if (typeName == "Custom")
        {
            return new Custom(node);
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