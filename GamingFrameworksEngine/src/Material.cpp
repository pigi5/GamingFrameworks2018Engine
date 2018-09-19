#include "../header/Material.h"

std::map<const std::string, const Material*> Material::objectMap;

Material::Material(const YAML::Node& config)
{
    name = config["name"].as<std::string>();

    if (config["friction"])
    {
        friction = config["friction"].as<float>();
    }
}

std::ostream& operator<<(std::ostream& output, const Material& object)
{
    output << " name: " << object.name;
    output << " friction: " << object.friction;
    return output;
}