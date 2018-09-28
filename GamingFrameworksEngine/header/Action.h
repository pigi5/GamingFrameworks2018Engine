#pragma once

class Actor;

// implements the Visitor pattern
class Action 
{
public:
    virtual void run(Actor*) = 0;

    virtual YAML::Emitter& serialize(YAML::Emitter&) const = 0;

    friend YAML::Emitter& operator<<(YAML::Emitter& out, const Action& obj)
    {
        return obj.serialize(out);
    }
};
