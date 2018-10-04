#include "../header/Material.h"

const std::string Material::DIR_NAME = "materials";

std::map<const std::string, Material*> Material::objectMap;

Material::Material()
{
    name = "unknown";
    friction = 0.0f;
}

Material::Material(const YAML::Node& config, bool shallow)
{
    name = config["name"].as<std::string>();
    friction = config["friction"].as<float>();
}

std::ostream& operator<<(std::ostream& output, const Material& object)
{
    output << " name: " << object.name;
    output << " friction: " << object.friction;
    return output;
}