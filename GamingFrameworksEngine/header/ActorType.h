#pragma once

#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
#include "../header/Configurable.h"
#include "../header/Trigger.h"
#include "../header/Action.h"
#include "yaml-cpp/yaml.h"
#include "Sprite.h"

class ActorType
{
private:

public:
    static const std::string DIR_NAME;
    static std::unordered_map<std::string, ActorType*> objectMap;

    static void createActorType(std::string);
    
    std::string name;
    float friction;
    float maxXSpeed;
    float maxYSpeed;
    bool gravitous;
    bool collidable;
	Sprite* sprite;
    float xScale;
    float yScale;
    float imageSpeed;
    float depth;
    std::unordered_map<std::string, int> attributes;
    // triggers/actions
    std::unordered_map<Trigger*, std::list<Action*>, TriggerHash, TriggerEquals> actionMap;

    ActorType(std::string);
    ActorType(const YAML::Node&, bool);
    ~ActorType();
    bool operator<(const ActorType& other) const;
    size_t hashCode() const;
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const ActorType& obj);
    friend Logger& operator<<(Logger& logger, const ActorType& obj);
    const std::string toString() const;
};