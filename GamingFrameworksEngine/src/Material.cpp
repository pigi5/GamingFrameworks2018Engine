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

YAML::Emitter& operator<<(YAML::Emitter& out, const Material& obj)
{ 
    out << YAML::Key << "name" << YAML::Value << obj.name;
    out << YAML::Key << "friction" << YAML::Value << obj.friction;
    return out;
}