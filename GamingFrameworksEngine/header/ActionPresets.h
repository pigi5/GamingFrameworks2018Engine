#pragma once

#include <cmath>
#include "Action.h"
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
        ApplyForce(float acceleration)
        {
            this->xAcceleration = xAcceleration;
            this->yAcceleration = yAcceleration;
        }

        ApplyForce(const YAML::Node& node)
        {
            xAcceleration = node["xAcceleration"].as<float>();
            yAcceleration = node["yAcceleration"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        out << YAML::Key << "type" << YAML::Value << "ApplyForce";
	        out << YAML::Key << "xAcceleration" << YAML::Value << xAcceleration;
	        out << YAML::Key << "yAcceleration" << YAML::Value << yAcceleration;
	        return out;
        }
    
        void run(Actor* actor)
        {
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
    
    // instantly move to the position of the nearest actor of the given type
    class MoveToNearest : public Action
    {
    private:
        ActorType* targetType;
    public:
        MoveToNearest(ActorType* targetType)
        {
            this->targetType = targetType;
        }

        MoveToNearest(const YAML::Node& node)
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
	        out << YAML::Key << "type" << YAML::Value << "MoveToNearest";
	        out << YAML::Key << "target" << YAML::Value << targetType->name;
	        return out;
        }
    
        void run(Actor* actor)
        {
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
        MoveTowardNearest(ActorType* targetType, float speed)
        {
            this->targetType = targetType;
            this->speed = speed;
        }

        MoveTowardNearest(const YAML::Node& node)
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
	        out << YAML::Key << "type" << YAML::Value << "MoveTowardNearest";
	        out << YAML::Key << "target" << YAML::Value << targetType->name;
	        out << YAML::Key << "speed" << YAML::Value << speed;
	        return out;
        }
    
        void run(Actor* actor)
        {
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
        MoveTo(float xPosition, float yPosition)
        {
            this->xPosition = xPosition;
            this->yPosition = yPosition;
        }

        MoveTo(const YAML::Node& node)
        {
            xPosition = node["xPosition"].as<float>();
            yPosition = node["yPosition"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        out << YAML::Key << "type" << YAML::Value << "MoveTo";
	        out << YAML::Key << "xPosition" << YAML::Value << xPosition;
	        out << YAML::Key << "yPosition" << YAML::Value << yPosition;
	        return out;
        }
    
        void run(Actor* actor)
        {
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
        Move(float xOffset, float yOffset)
        {
            this->xOffset = xOffset;
            this->yOffset = yOffset;
        }

        Move(const YAML::Node& node)
        {
            xOffset = node["xOffset"].as<float>();
            yOffset = node["yOffset"].as<float>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        out << YAML::Key << "type" << YAML::Value << "Move";
	        out << YAML::Key << "xOffset" << YAML::Value << xOffset;
	        out << YAML::Key << "yOffset" << YAML::Value << yOffset;
	        return out;
        }
    
        void run(Actor* actor)
        {
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
        Create(const ActorType* actorType, State& startState)
        {
            this->actorType = actorType;
            this->startState = startState;
        }

        Create(const YAML::Node& node)
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
	        out << YAML::Key << "type" << YAML::Value << "Create";
	        out << YAML::Key << "target" << YAML::Value << actorType;
	        out << startState;
	        return out;
        }
    
        void run(Actor* actor)
        {
            Actor* newActor = new Actor(actor->getRoom(), actorType, startState);
            actor->getRoom()->addActor(newActor);
        }
    };
    
    // destroys the actor
    class Destroy : public Action
    {
    private:
    public:
        Destroy()
        {
        }

        Destroy(const YAML::Node& node)
        {
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        out << YAML::Key << "type" << YAML::Value << "Destroy";
	        return out;
        }
    
        void run(Actor* actor)
        {
            actor->getRoom()->deleteActor(actor);
        }
    };
    
    // starts a timer
    class SetTimer : public Action
    {
    private:
        int index;
        float time;
    public:
        SetTimer(int index, float time)
        {
            this->index = index;
            this->time = time;
        }

        SetTimer(const YAML::Node& node)
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
	        out << YAML::Key << "type" << YAML::Value << "SetTimer";
	        out << YAML::Key << "index" << YAML::Value << index;
	        out << YAML::Key << "time" << YAML::Value << time;
	        return out;
        }
    
        void run(Actor* actor)
        {
            actor->getRoom()->startTimer(index, time);
        }
    };
    
    // fires a custom trigger
    class CallCustom : public Action
    {
    private:
        int index;
    public:
        CallCustom(int index)
        {
            this->index = index;
        }

        CallCustom(const YAML::Node& node)
        {
            this->index = node["index"].as<int>();
        }
   
        YAML::Emitter& serialize(YAML::Emitter& out) const
        {
	        out << YAML::Key << "type" << YAML::Value << "CallCustom";
	        out << YAML::Key << "index" << YAML::Value << index;
	        return out;
        }
    
        void run(Actor* actor)
        {
            trigger_preset::Custom trigger(&Index(index));
            actor->getRoom()->fireTrigger(trigger);
        }
    };

    // abstract factory (**add new actions here**)
    static Action* createAction(const std::string typeName, const YAML::Node& node)
    {
        if (typeName == "ApplyForce")
        {
            return new ApplyForce(node);
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
        else
        {
            std::stringstream errorMessage;
            errorMessage << "Action " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
    }
}