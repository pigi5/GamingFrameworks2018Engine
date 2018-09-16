#pragma once

#include <functional>

// Base class for triggers to be treated polymorphically
class Trigger 
{
public:
    virtual bool operator<(const Trigger& other) const = 0;
};

// All explicit triggers should inherit from this
// T is the type of the Trigger ID, which is used as the key in a mapping.
// Because of this, T must implement the < operator
template <typename T>
class TriggerType : public Trigger
{
private:
    T* id;
public:
    TriggerType(T* id)
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
};

// Compares Trigger pointers (by deferencing them)
// This is useful for stl collections that need a less-than operator for Trigger pointers
struct TriggerComparator : std::binary_function<const Trigger*, const Trigger*, bool>
{
    bool operator() (const Trigger* a, const Trigger* b) const 
    {
        return *a < *b;
    }
};