#pragma once
#include "Sprite.h"

class Drawable
{
private:
    Sprite sprite;
    float imageSpeed;
    int imageFrame;
    int imageNum;
    float xImageOffset;
    float yImageOffset;
    float imageAngle;

public:
    Drawable();
    ~Drawable();
    void draw();
};

