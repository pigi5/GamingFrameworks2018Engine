#pragma once

#include <map>
#include <list>
#include <iostream>
#include "../header/Configurable.h"
#include "../header/Trigger.h"
#include "../header/Action.h"
#include "../header/Material.h"
#include "yaml-cpp/yaml.h"
#include "Sprite.h"

class ActorType
{
private:

public:
    static const std::string DIR_NAME;
    static std::map<const std::string, ActorType*> objectMap;

    static void createActorType(std::string);
    
    std::string name;
    const Material* material;
    float maxXSpeed;
    float maxYSpeed;
    bool gravitous;
	Sprite* sprite;
    float xScale;
    float yScale;
    float imageSpeed;
    std::map<const std::string, int> attributes;
    // triggers/actions
    std::map<const Trigger*, std::list<Action*>> actionMap;

    ActorType(std::string);
    ActorType(const YAML::Node&, bool);
    ~ActorType();
    bool operator<(const ActorType& other) const;
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const ActorType& obj);
};