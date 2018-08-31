#pragma once

#include <vector>
#include "Rectangle.h"

// Holds an animation and a hitbox for objects to use.
class Sprite
{
private:
    //std::vector<Image*> frames;
    Rectangle* hitbox;
public:
    Sprite();
    ~Sprite();

    Rectangle* getHitbox() const;
};