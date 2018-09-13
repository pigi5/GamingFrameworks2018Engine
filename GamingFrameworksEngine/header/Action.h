#pragma once

class Actor;

// implements the Visitor pattern
class Action 
{
public:
    virtual void run(Actor*) = 0;
};
