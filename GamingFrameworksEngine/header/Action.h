#pragma once

#include <string>
#include "yaml-cpp/yaml.h"
#include "Logger.h"

class Actor;

enum Comparison
{
    EQUAL, NOT_EQUAL, LESS_THAN, LESS_THAN_EQUAL, GREATER_THAN, GREATER_THAN_EQUAL
};

class Conditional
{
public:
    Comparison comparison;
    std::string key;
    int value;

    Conditional(const YAML::Node& node);
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const Conditional& obj);
};

// implements the Visitor pattern
class Action 
{
public:
    std::list<Conditional*> conditionals;
    
    Action(){};

    Action(const YAML::Node& node);

    ~Action()
    {
        for (Conditional* conditional : conditionals)
        {
            delete conditional;
        }
        conditionals.clear();
    }
    
    virtual void run(Actor* actor) = 0;

    virtual bool checkConditionals(Actor* actor);

    virtual std::string getTypeName() const = 0;

    virtual const std::string& toString() const = 0;

    virtual YAML::Emitter& serialize(YAML::Emitter& out) const;
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const Action& obj);
    friend Logger& operator<<(Logger& logger, const Action& obj)
    {
        logger << obj.getTypeName();
        return logger;
    }
};
