#pragma once

#include <map>
#include <string>
#include <iostream>
#include "yaml-cpp/yaml.h"

class Material
{
public:
    static std::map<const std::string, Material*> objectMap;

    std::string name;
    float friction;
    
    Material();
    Material(const YAML::Node&, bool);
    friend std::ostream& operator<<(std::ostream&, const Material&);
};