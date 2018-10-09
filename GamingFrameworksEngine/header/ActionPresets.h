#pragma once

#include <cmath>
#include "Action.h"
#include "Engine.h"
#include "Room.h"
#include "Actor.h"
#include "Utils.h"
#include "yaml-cpp/yaml.h"

namespace action_preset
{
    class ApplyForce : public Action
    {
    private:
        float xAcceleration;
        float yAcceleration;
    public:
        std::string getTypeName() const
        {
            return "ApplyForce";
        }

        ApplyForce(float xAcceleration, float yAcceleration)
        {
            this->xAcceleration = xAcceleration;
            this->yAcceleration = yAcceleration;
        }

        ApplyForce(const YAML::Node& node) : Action(node)
        {
            xAcceleration = node["xAcceleration"].as<float>();
            yAcceleration = node["yAcceleration"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "xAcceleration" << YAML::Value << xAcceleration;
	        out << YAML::Key << "yAcceleration" << YAML::Value << yAcceleration;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + std::to_string(xAcceleration) + " - " + std::to_string(yAcceleration);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            if (xAcceleration != 0)
            {
                actor->setXAcceleration(xAcceleration);
            }
            if (yAcceleration != 0)
            {
                actor->setYAcceleration(yAcceleration);
            }
        }
    };

    class SetXSpeed : public Action
    {
    private:
        float speed;
    public:
        std::string getTypeName() const
        {
            return "SetXSpeed";
        }

        SetXSpeed(float speed)
        {
            this->speed = speed;
        }

        SetXSpeed(const YAML::Node& node) : Action(node)
        {
            speed = node["speed"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "speed" << YAML::Value << speed;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + std::to_string(speed);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->setXSpeed(speed);
        }
    };

    class SetYSpeed : public Action
    {
    private:
        float speed;
    public:
        std::string getTypeName() const
        {
            return "SetXSpeed";
        }

        SetYSpeed(float speed)
        {
            this->speed = speed;
        }

        SetYSpeed(const YAML::Node& node) : Action(node)
        {
            speed = node["speed"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "speed" << YAML::Value << speed;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + std::to_string(speed);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->setYSpeed(speed);
        }
    };
    
    // instantly move to the position of the nearest actor of the given type
    class MoveToNearest : public Action
    {
    private:
        ActorType* targetType;
    public:
        std::string getTypeName() const
        {
            return "MoveToNearest";
        }

        MoveToNearest(ActorType* targetType)
        {
            this->targetType = targetType;
        }

        MoveToNearest(const YAML::Node& node) : Action(node)
        {
            std::string typeName = node["target"].as<std::string>();
            auto mapItem = ActorType::objectMap.find(typeName);
            if (mapItem == ActorType::objectMap.end())
            {
                std::stringstream errorMessage;
                errorMessage << "Actor Type " << typeName << " does not exist.";
                throw ConfigurationError(errorMessage.str());
            }

            this->targetType = mapItem->second;
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "target" << YAML::Value << targetType->name;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + targetType->toString();
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            auto nearest = engine_util::findNearest(actor, actor->getRoom()->getActors());
            actor->setPosition(nearest.first->getState().xPosition, nearest.first->getState().yPosition);
        }
    };
    
    // set the speed of the actor to move at the given speed to the 
    // position of the nearest actor of the given type
    class MoveTowardNearest : public Action
    {
    private:
        ActorType* targetType;
        float speed;
    public:
        std::string getTypeName() const
        {
            return "MoveTowardNearest";
        }

        MoveTowardNearest(ActorType* targetType, float speed)
        {
            this->targetType = targetType;
            this->speed = speed;
        }

        MoveTowardNearest(const YAML::Node& node) : Action(node)
        {
            std::string typeName = node["target"].as<std::string>();
            auto mapItem = ActorType::objectMap.find(typeName);
            if (mapItem == ActorType::objectMap.end())
            {
                std::stringstream errorMessage;
                errorMessage << "Actor Type " << typeName << " does not exist.";
                throw ConfigurationError(errorMessage.str());
            }

            this->targetType = mapItem->second;
            this->speed = node["speed"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "target" << YAML::Value << targetType->name;
	        out << YAML::Key << "speed" << YAML::Value << speed;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + targetType->toString() + " - " + std::to_string(speed);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            auto nearest = engine_util::findNearest(actor, actor->getRoom()->getActors());
            
            State distanceVector = nearest.first->getState() - actor->getState();
            // special case to avoid divide-by-zero
            if (distanceVector.xPosition == 0)
            {
                actor->setXSpeed(0);
                actor->setYSpeed(speed);
                return;
            }
            // calculate speeds
            float angle = atan(distanceVector.yPosition / distanceVector.xPosition);
            float xSpeed = speed * cos(angle) * engine_util::sign(distanceVector.yPosition);
            float ySpeed = speed * sin(angle) * engine_util::sign(distanceVector.yPosition);

            actor->setXSpeed(xSpeed);
            actor->setYSpeed(ySpeed);
        }
    };
    
    // move the actor to the given location
    class MoveTo : public Action
    {
    private:
        float xPosition;
        float yPosition;
    public:
        std::string getTypeName() const
        {
            return "MoveTo";
        }

        MoveTo(float xPosition, float yPosition)
        {
            this->xPosition = xPosition;
            this->yPosition = yPosition;
        }

        MoveTo(const YAML::Node& node) : Action(node)
        {
            xPosition = node["xPosition"].as<float>();
            yPosition = node["yPosition"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "xPosition" << YAML::Value << xPosition;
	        out << YAML::Key << "yPosition" << YAML::Value << yPosition;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + std::to_string(xPosition) + " - " + std::to_string(yPosition);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->setPosition(xPosition, yPosition);
        }
    };
    
    // move the actor a given offset amount
    class Move : public Action
    {
    private:
        float xOffset;
        float yOffset;
    public:
        std::string getTypeName() const
        {
            return "Move";
        }

        Move(float xOffset, float yOffset)
        {
            this->xOffset = xOffset;
            this->yOffset = yOffset;
        }

        Move(const YAML::Node& node) : Action(node)
        {
            xOffset = node["xOffset"].as<float>();
            yOffset = node["yOffset"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "xOffset" << YAML::Value << xOffset;
	        out << YAML::Key << "yOffset" << YAML::Value << yOffset;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + std::to_string(xOffset) + " - " + std::to_string(yOffset);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->offset(xOffset, yOffset);
        }
    };
    
    // spawn a new actor of the given type
    class Create : public Action
    {
    private:
        const ActorType* actorType;
        State startState;
    public:
        std::string getTypeName() const
        {
            return "Create";
        }

        Create(const ActorType* actorType, State& startState)
        {
            this->actorType = actorType;
            this->startState = startState;
        }

        Create(const YAML::Node& node) : Action(node)
        {
            std::string typeName = node["target"].as<std::string>();
            auto mapItem = ActorType::objectMap.find(typeName);
            if (mapItem == ActorType::objectMap.end())
            {
                std::stringstream errorMessage;
                errorMessage << "Actor Type " << typeName << " does not exist.";
                throw ConfigurationError(errorMessage.str());
            }
            this->actorType = mapItem->second;

            this->startState = State(node);
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "target" << YAML::Value << actorType;
	        out << startState;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + actorType->toString() + " - " + startState.toString();
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            Actor* newActor = new Actor(actor->getRoom(), actorType, startState);
            actor->getRoom()->addActor(newActor);
        }
    };
    
    // destroys the actor
    class Destroy : public Action
    {
    private:
    public:
        std::string getTypeName() const
        {
            return "Destroy";
        }

        Destroy(const YAML::Node& node) : Action(node)
        {
        }
        
        const std::string toString() const
        {
            return getTypeName();
        }

        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->getRoom()->deleteActor(actor);
        }
    };
    
    // sets an attribute
    class AttributeSet : public Action
    {
    private:
        std::string key;
        int value;
    public:
        std::string getTypeName() const
        {
            return "AttributeSet";
        }

        AttributeSet(std::string key, int value)
        {
            this->key = key;
            this->value = value;
        }

        AttributeSet(const YAML::Node& node) : Action(node)
        {
            this->key = node["attribute"].as<std::string>();
            this->value = node["value"].as<int>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "attribute" << YAML::Value << key;
	        out << YAML::Key << "value" << YAML::Value << value;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + key + " - " + std::to_string(value);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->setAttribute(key, value);
        }
    };
    
    // changes an attribute by offset
    class AttributeChange : public Action
    {
    private:
        std::string key;
        int offset;
    public:
        std::string getTypeName() const
        {
            return "AttributeChange";
        }

        AttributeChange(std::string key, int offset)
        {
            this->key = key;
            this->offset = offset;
        }

        AttributeChange(const YAML::Node& node) : Action(node)
        {
            this->key = node["attribute"].as<std::string>();
            this->offset = node["offset"].as<int>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "attribute" << YAML::Value << key;
	        out << YAML::Key << "offset" << YAML::Value << offset;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + key + " - " + std::to_string(offset);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->changeAttribute(key, offset);
        }
    };
    
    // starts a timer
    class SetTimer : public Action
    {
    private:
        int index;
        float time;
    public:
        std::string getTypeName() const
        {
            return "SetTimer";
        }

        SetTimer(int index, float time)
        {
            this->index = index;
            this->time = time;
        }

        SetTimer(const YAML::Node& node) : Action(node)
        {
            this->index = node["index"].as<int>();

            if (index < 0 || index >= Room::NUM_TIMERS)
            {
                std::stringstream errorMessage;
                errorMessage << "Timer index " << index << " out of bounds.";
                throw ConfigurationError(errorMessage.str());
            }

            this->time = node["time"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "index" << YAML::Value << index;
	        out << YAML::Key << "time" << YAML::Value << time;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + std::to_string(index) + " - " + std::to_string(time);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->getRoom()->startTimer(index, time);
        }
    };
    
    // fires a custom trigger
    class CallCustom : public Action
    {
    private:
        int index;
    public:
        std::string getTypeName() const
        {
            return "CallCustom";
        }

        CallCustom(int index)
        {
            this->index = index;
        }

        CallCustom(const YAML::Node& node) : Action(node)
        {
            this->index = node["index"].as<int>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "index" << YAML::Value << index;
	        return out;
        }
        
        const std::string toString() const
        {
            return getTypeName() + " - " + std::to_string(index);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            Index wrapper(index);
            trigger_preset::Custom trigger(&wrapper);
            actor->getRoom()->fireTrigger(&trigger);
        }
    };
    
    // change room index
    class ChangeRoom : public Action
    {
    private:
        int offset;
    public:
        std::string getTypeName() const
        {
            return "ChangeRoom";
        }

        ChangeRoom(int offset)
        {
            this->offset = offset;
        }

        ChangeRoom(const YAML::Node& node) : Action(node)
        {
            this->offset = node["offset"].as<int>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "offset" << YAML::Value << offset;
	        return out;
        }
        
        const std::string& toString() const
        {
            return getTypeName() + " - " + std::to_string(offset);
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->getRoom()->getEngine()->changeRoom(offset);
        }
    };
    
    // set room index
    class SetRoom : public Action
    {
    private:
        std::string name;
    public:
        std::string getTypeName() const
        {
            return "SetRoom";
        }

        SetRoom(int index)
        {
            this->name = name;
        }

        SetRoom(const YAML::Node& node) : Action(node)
        {
            this->name = node["target"].as<int>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        Action::serialize(out);
	        out << YAML::Key << "target" << YAML::Value << name;
	        return out;
        }
        
        const std::string& toString() const
        {
            return getTypeName() + " - " + name;
        }
    
        void run(Actor* actor)
        {
            if (!checkConditionals(actor)) return;

            actor->getRoom()->getEngine()->setRoom(name);
        }
    };

    // abstract factory (**add new actions here**)
    static Action* createAction(const std::string typeName, const YAML::Node& node)
    {
        if (typeName == "ApplyForce")
        {
            return new ApplyForce(node);
        }
        else if (typeName == "SetXSpeed")
        {
            return new SetXSpeed(node);
        }
        else if (typeName == "SetYSpeed")
        {
            return new SetYSpeed(node);
        }
        else if (typeName == "MoveToNearest")
        {
            return new MoveToNearest(node);
        }
        else if (typeName == "MoveTowardNearest")
        {
            return new MoveTowardNearest(node);
        }
        else if (typeName == "MoveTo")
        {
            return new MoveTo(node);
        }
        else if (typeName == "Move")
        {
            return new Move(node);
        }
        else if (typeName == "Create")
        {
            return new Create(node);
        }
        else if (typeName == "AttributeSet")
        {
            return new AttributeSet(node);
        }
        else if (typeName == "AttributeChange")
        {
            return new AttributeChange(node);
        }
        else if (typeName == "Destroy")
        {
            return new Destroy(node);
        }
        else if (typeName == "SetTimer")
        {
            return new SetTimer(node);
        }
        else if (typeName == "CallCustom")
        {
            return new CallCustom(node);
        }
        else if (typeName == "ChangeRoom")
        {
            return new ChangeRoom(node);
        }
        else if (typeName == "SetRoom")
        {
            return new SetRoom(node);
        }
        else
        {
            std::stringstream errorMessage;
            errorMessage << "Action " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
    }
}