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

    Material(const YAML::Node&);
    friend std::ostream& operator<<(std::ostream&, const Material&);
};