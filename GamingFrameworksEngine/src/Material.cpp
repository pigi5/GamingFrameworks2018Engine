#include "../header/Material.h"
#include "../header/Configurable.h"

const std::string Material::DIR_NAME = "materials";
std::unordered_map<std::string, Material*> Material::objectMap;

void Material::createMaterial(std::string name)
{
    Material* newType = new Material(name);
    
    // add material to map, keyed by name
    if (!objectMap.emplace(newType->name, newType).second)
    {
        // if key already existed, throw error
        std::stringstream errorMessage;
        errorMessage << "Material name \"" << newType->name << "\" is not unique.";
        throw ConfigurationError(errorMessage.str());
    }
}

Material::Material(std::string name)
{
    this->name = name;
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

Logger& operator<<(Logger& logger, const Material& obj) 
{
    logger << obj.name;
    return logger;
}