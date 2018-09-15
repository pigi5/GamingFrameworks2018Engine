#pragma once

class Actor;

// implements the Visitor pattern
class Action 
{
public:
    virtual void run(Actor* ) const = 0;
};
