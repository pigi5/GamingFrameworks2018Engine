#include "../header/ActorType.h"
#include "yaml-cpp/yaml.h"

ActorType::ActorType(std::string filename)
{
    YAML::Node config = YAML::LoadFile(filename);

    // TODO continue parsing actor type
};