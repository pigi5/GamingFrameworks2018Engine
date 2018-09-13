#pragma once

// Base class for triggers to be treated polymorphically
class Trigger 
{
public:
    virtual bool operator<(const Trigger& other) const = 0;
};

// All explicit triggers should inherit from this
template <typename T>
class TriggerType : public Trigger
{
private:
    T* data;
public:
    TriggerType(T* data)
    {
        this->data = data;
    }
    
    // If the triggers are the same type, compare their data, otherwise compare addresses
    bool operator<(const Trigger& other) const
    {
        if (typeid(*this) == typeid(other))
        {
            return *data < *(static_cast<const TriggerType&>(other).data);
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