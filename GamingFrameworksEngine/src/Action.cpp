#include "../header/Action.h"
#include "../header/Configurable.h"
#include "../header/Actor.h"
#include <sstream>

Comparison createComparison(std::string name)
{
    if (name == "eq")
    {
        return EQUAL;
    } 
    else if (name == "ne")
    {
        return NOT_EQUAL;
    }
    else if (name == "lt")
    {
        return LESS_THAN;
    }
    else if (name == "le")
    {
        return LESS_THAN_EQUAL;
    }
    else if (name == "gt")
    {
        return GREATER_THAN;
    }
    else if (name == "ge")
    {
        return GREATER_THAN_EQUAL;
    }
    std::stringstream errorMessage;
    errorMessage << "Comparision " << name << " does not exist.";
    throw ConfigurationError(errorMessage.str());
}

const std::string getComparisonString(const Comparison& obj)
{
    switch (obj)
    {
    case EQUAL:
        return "eq";
    case NOT_EQUAL:
        return "ne";
    case LESS_THAN:
        return "lt";
    case LESS_THAN_EQUAL:
        return "le";
    case GREATER_THAN:
        return "gt";
    case GREATER_THAN_EQUAL:
        return "ge";
    default:
        throw ConfigurationError("Comparision string not implemented.");
    }
}

const std::string getComparisonSymbol(const Comparison& obj)
{
    switch (obj)
    {
    case EQUAL:
        return "=";
    case NOT_EQUAL:
        return "=/=";
    case LESS_THAN:
        return "<";
    case LESS_THAN_EQUAL:
        return "<=";
    case GREATER_THAN:
        return ">";
    case GREATER_THAN_EQUAL:
        return ">=";
    default:
        throw ConfigurationError("Comparision symbol not implemented.");
    }
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Comparison& obj)
{
    out << getComparisonString(obj);
    return out;
}

Conditional::Conditional(const YAML::Node& node)
{
    comparison = createComparison(node["comparison"].as<std::string>());
    key = node["attribute"].as<std::string>();
    value = node["value"].as<int>();
}

Conditional::Conditional(Comparison comparison, std::string key, int value)
{
	this->comparison = comparison;
	this->key = key;
	this->value = value;
}
    
YAML::Emitter& operator<<(YAML::Emitter& out, const Conditional& obj)
{
    out << YAML::Key << "comparison" << YAML::Value << obj.comparison;
    out << YAML::Key << "attribute" << YAML::Value << obj.key;
    out << YAML::Key << "value" << YAML::Value << obj.value;
    return out;
}
    
const std::string Conditional::toString() const
{
    return key + " " + getComparisonSymbol(comparison) + " " + std::to_string(value);
}

Action::Action(const YAML::Node& node)
{
    YAML::Node conditionalsNode = node["conditionals"];
    for (YAML::Node conditional : conditionalsNode)
    {
        conditionals.push_back(new Conditional(conditional));
    }
}

// returns true if all conditionals are met, otherwise false
bool Action::checkConditionals(Actor* actor)
{
    for (Conditional* conditional : conditionals)
    {
        int checkValue = actor->getAttribute(conditional->key);
        switch (conditional->comparison)
        {
        case EQUAL:
            if (!(checkValue == conditional->value))
            {
                return false;
            }
            break;
        case NOT_EQUAL:
            if (!(checkValue != conditional->value))
            {
                return false;
            }
            break;
        case LESS_THAN:
            if (!(checkValue < conditional->value))
            {
                return false;
            }
            break;
        case LESS_THAN_EQUAL:
            if (!(checkValue <= conditional->value))
            {
                return false;
            }
            break;
        case GREATER_THAN:
            if (!(checkValue > conditional->value))
            {
                return false;
            }
            break;
        case GREATER_THAN_EQUAL:
            if (!(checkValue >= conditional->value))
            {
                return false;
            }
            break;
        default:
            throw ConfigurationError("Comparision function not implemented.");
        }
    }
    return true;
}

YAML::Emitter& Action::serialize(YAML::Emitter& out) const
{
	out << YAML::Key << "type" << YAML::Value << getTypeName();
	out << YAML::Key << "conditionals" << YAML::Value << YAML::BeginSeq;
    for (Conditional* conditional : conditionals)
    {
        out << YAML::BeginMap;
        out << *conditional;
        out << YAML::EndMap;
    }
	out << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Action& obj)
{
    return obj.serialize(out);
}