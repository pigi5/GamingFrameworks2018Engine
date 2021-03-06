#pragma once

#include <functional>
#include <string>
#include "Logger.h"
#include "Utils.h"

// Base class for triggers to be treated polymorphically
class Trigger 
{
public:
    virtual bool operator<(const Trigger& other) const = 0;
    virtual YAML::Emitter& serialize(YAML::Emitter&) const = 0;
    virtual Logger& serializeDebug(Logger&) const = 0;
    virtual std::string getTypeName() const = 0;
    virtual const std::string toString() const = 0;
    virtual size_t hashCode() const = 0;

    friend YAML::Emitter& operator<<(YAML::Emitter& out, const Trigger& obj)
    {
        return obj.serialize(out);
    }

    friend Logger& operator<<(Logger& logger, const Trigger& obj)
    {
        return obj.serializeDebug(logger);
    }
};

// All explicit triggers should inherit from this
// T is the type of the Trigger ID, which is used as the key in a mapping.
// Because of this, T must implement the < operator
template <typename T>
class TriggerType : public Trigger
{
public:
    const T* id;
    TriggerType(const T* id)
    {
        this->id = id;
    }
    
    // If the triggers are the same type, compare their data, otherwise compare addresses
    bool operator<(const Trigger& other) const
    {
        if (typeid(*this) == typeid(other))
        {
            return *id < *(static_cast<const TriggerType&>(other).id);
        }
        return this < &other;
    }
    
    size_t hashCode() const
    {
        return id->hashCode();
    }

    YAML::Emitter& serialize(YAML::Emitter& out) const
    {
        out << YAML::Key << "type" << YAML::Value << getTypeName();
        out << *id;
        return out;
    }

    Logger& serializeDebug(Logger& logger) const
    {
        logger << getTypeName() << " - id - ";
        if (id == NULL)
        {
            logger << "null";
        }
        else
        {
            logger << *id;
        }
        return logger;
    }

    const std::string toString() const
    {
        return getTypeName() + ": {" + id->toString() + "}";
    }
};

// Hashes Trigger pointers (by deferencing them)
// This is useful for stl collections that need a hash for Trigger pointers
struct TriggerHash
{
    size_t operator()(Trigger* obj) const 
    {
        std::hash<std::string> hasher;
        int lhs = hasher(obj->getTypeName());
        int rhs = obj->hashCode();
        lhs^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        return lhs;
    }
};

// Compares Trigger pointers (by deferencing them)
// This is useful for stl collections that need an == for Trigger pointers
struct TriggerEquals : public std::binary_function<Trigger*, Trigger*, bool>
{
    bool operator()(Trigger* a, Trigger* b) const 
    {
        return a->getTypeName() == b->getTypeName() && a->hashCode() == b->hashCode();
    }
};