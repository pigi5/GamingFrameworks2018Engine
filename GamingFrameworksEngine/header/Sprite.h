#pragma once

#include <vector>
#include "Rectangle.h"

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