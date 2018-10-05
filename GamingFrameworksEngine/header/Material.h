#pragma once

#include <map>
#include <string>
#include <iostream>
#include "yaml-cpp/yaml.h"

class Material
{
public:
    static const std::string DIR_NAME;
    static std::map<const std::string, Material*> objectMap;

    std::string name;
    float friction;
    
    static void createMaterial(std::string);
    
    Material(std::string);
    Material(const YAML::Node&, bool);
    friend YAML::Emitter& operator<<(YAML::Emitter&, const Material&);
};