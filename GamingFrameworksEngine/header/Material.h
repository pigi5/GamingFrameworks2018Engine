#pragma once

#include <unordered_map>
#include <string>
#include <string>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "Logger.h"

class Material
{
public:
    static const std::string DIR_NAME;
    static std::unordered_map<std::string, Material*> objectMap;

    std::string name;
    float friction;
    
    static void createMaterial(std::string);
    
    Material(std::string);
    Material(const YAML::Node&, bool);
    friend YAML::Emitter& operator<<(YAML::Emitter&, const Material&);
    friend Logger& operator<<(Logger&, const Material&);
};