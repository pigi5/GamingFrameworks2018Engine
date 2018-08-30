#include "../header/Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

Rectangle* Sprite::getHitbox() const
{
    return hitbox;
}