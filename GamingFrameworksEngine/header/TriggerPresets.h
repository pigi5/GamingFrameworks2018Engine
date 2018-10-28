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
        return TriggerType::serialize(out);                         \
    }                                                               \
    std::string getTypeName() const                                 \
    {                                                               \
        return #_name;                                              \
    }                                                               \
};                                                                  \

#include "Trigger.h"
#include "Actor.h"
#include "Overlay.h"
#include "yaml-cpp/yaml.h"
#include "Configurable.h"
#include "ButtonStates.h"
#include "MouseStates.h"
#include "KeyMap.h"

// ID structures //

struct ButtonInputType 
{
	short id;
	ButtonState::ButtonState state;

	bool operator<(const ButtonInputType& other) const
	{
		return id * 3 + state < other.id * 3 + other.state;
	}

	size_t hashCode() const
	{
		return id * 3 + state;
	}

    ButtonInputType() {}

    ButtonInputType(const YAML::Node& config)
    {
        id = config["id"].as<short>();
        state = ButtonInputType::parseState(config["state"].as<std::string>());
    }
    
    static ButtonState::ButtonState parseState(std::string state)
    {
        if (state != "up" && state != "down" && state != "hold")
        {
            throw ConfigurationError("Button Input Type state must be \"down\", \"up\" or \"hold\"");
        }
		if (state == "up")
			return ButtonState::RELEASE;
		else if (state == "down")
			return ButtonState::PRESS;
		else
			return ButtonState::HOLD;
    }
    
    static std::string emitState(ButtonState::ButtonState bs)
    {
		string state = "";
		switch (bs)
		{
		case ButtonState::PRESS:
			state = "down";
			break;
		case ButtonState::RELEASE:
			state = "up";
			break;
		case ButtonState::HOLD:
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
    
    friend Logger& operator<<(Logger& logger, const ButtonInputType& obj)
    {
        logger << "{id: " << obj.id << ", state: " << ButtonInputType::emitState(obj.state) << "}";
        return logger;
    }
    
    const std::string toString() const
    {
        std::string repr = "button: " + keyNames[id] + ", state: " + ButtonInputType::emitState(state);
        return repr;
    }
};

struct MouseInputType 
{
	MouseState::MouseState state;
    
	bool operator<(const MouseInputType& other) const
	{
		return state < other.state;
	}

	size_t hashCode() const
	{
		return state;
	}

	MouseInputType() {}

    MouseInputType(const YAML::Node& config)
    {
        state = MouseInputType::parseState(config["state"].as<std::string>());
    }
    
    static MouseState::MouseState parseState(std::string state)
    {
        if (state != "up" && state != "down" && state != "down on" && state != "up on")
        {
            throw ConfigurationError("Mouse Input Type state must be \"down\", \"up\", \"down on\", or \"up on\"");
        }
		if (state == "up")
        {
			return MouseState::RELEASE;
        }
		else if (state == "down")
        {
			return MouseState::PRESS;
        }
		else if (state == "up on")
        {
			return MouseState::RELEASE_ON;
        }
		else
        {
			return MouseState::PRESS_ON;
        }
    }
    
    static std::string emitState(MouseState::MouseState bs)
    {
		string state = "";
		switch (bs)
		{
        case MouseState::PRESS:
			state = "down";
			break;
		case MouseState::RELEASE:
			state = "up";
			break;
		case MouseState::PRESS_ON:
			state = "down on";
			break;
		case MouseState::RELEASE_ON:
			state = "up on";
			break;
		default:
			break;
		}
        return state;
    }
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const MouseInputType& obj)
    {
        out << YAML::Key << "state" << YAML::Value << MouseInputType::emitState(obj.state);
        return out;
    }
    
    friend Logger& operator<<(Logger& logger, const MouseInputType& obj)
    {
        logger << "{state: " << MouseInputType::emitState(obj.state) << "}";
        return logger;
    }
    
    const std::string toString() const
    {
        std::string repr = "state: " + MouseInputType::emitState(state);
        return repr;
    }
};


struct Index 
{
	int index;

	bool operator<(const Index& other) const
	{
		return index < other.index;
	}

	size_t hashCode() const
	{
		return index;
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
    
    friend Logger& operator<<(Logger& logger, const Index& obj)
    {
        logger << "{index: " << obj.index << "}";
        return logger;
    }
    
    const std::string toString() const
    {
        std::string repr = "index: " + std::to_string(index);
        return repr;
    }
};

struct ActorTypeWrapper 
{
	const ActorType* type;

	bool operator<(const ActorTypeWrapper& other) const
	{
		return *type < *(other.type);
	}

	size_t hashCode() const
	{
		return type->hashCode();
	}

    ActorTypeWrapper(const ActorType* type) 
    {
        this->type = type;
    }

    ActorTypeWrapper(const YAML::Node& config)
    {
        std::string typeName = config["target"].as<std::string>();
        auto mapAItem = ActorType::objectMap.find(typeName);
		auto mapOItem = OverlayType::objectMap.find(typeName);
        if (mapAItem == ActorType::objectMap.end() && mapOItem == OverlayType::objectMap.end())
        {
			std::stringstream errorMessage;
			errorMessage << "Actor/Overlay Type " << typeName << " does not exist.";
			throw ConfigurationError(errorMessage.str());
        }
		else if (mapAItem == ActorType::objectMap.end())
		{
			type = mapOItem->second;
		}
		else
		{
			type = mapAItem->second;
		}
    }
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const ActorTypeWrapper& obj)
    {
        out << YAML::Key << "target" << YAML::Value << obj.type->name;
        return out;
    }
    
    friend Logger& operator<<(Logger& logger, const ActorTypeWrapper& obj)
    {
        logger << "{target: " << *obj.type << "}";
        return logger;
    }
    
    const std::string toString() const
    {
        return "target: " + type->name;
    }
};

/*struct AudioWrapper
{
	const Audio* type;

	bool operator<(const AudioWrapper& other) const
	{
		return *type < *(other.type);
	}

	AudioWrapper(const Audio* type)
	{
		this->type = type;
	}

	AudioWrapper(const YAML::Node& config)
	{
		std::string typeName = config["target"].as<std::string>();
		auto mapItem = Audio::audioMap.find(typeName);
		if (mapItem == Audio::audioMap.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Audio " << typeName << " does not exist.";
			throw ConfigurationError(errorMessage.str());
		}
		type = mapItem->second;
	}

	friend YAML::Emitter& operator<<(YAML::Emitter& out, const AudioWrapper& obj)
	{
		out << YAML::Key << "type" << YAML::Value << obj.type;
		return out;
	}
};*/

// All trigger presets are defined here
namespace trigger_preset
{
    CREATE_TRIGGER(Collision, ActorTypeWrapper)
    CREATE_TRIGGER(ButtonInput, ButtonInputType)
    CREATE_TRIGGER(MouseInput, MouseInputType)
    CREATE_TRIGGER(Create, ActorTypeWrapper)
    CREATE_TRIGGER(Step, ActorTypeWrapper)
    CREATE_TRIGGER(Draw, ActorTypeWrapper)
    CREATE_TRIGGER(Destroy, ActorTypeWrapper)
    CREATE_TRIGGER(Timer, Index)
    CREATE_TRIGGER(Custom, Index)
	//CREATE_TRIGGER(EnterRoom, AudioWrapper)
	//CREATE_TRIGGER(LeaveRoom, AudioWrapper)


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
        else if (typeName == "MouseInput")
        {
            return new MouseInput(node);
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
		/*else if (typeName == "EnterRoom")
		{
			return new EnterRoom(node);
		}
		else if (typeName == "LeaveRoom")
		{
			return new LeaveRoom(node);
		}*/
        else
        {
            std::stringstream errorMessage;
            errorMessage << "Trigger " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
    }
}

#undef CREATE_TRIGGER