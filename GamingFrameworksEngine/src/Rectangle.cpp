#include "../header/Rectangle.h"

Rectangle::Rectangle(float x, float y, float width, float height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

bool Rectangle::collides(const Rectangle* other) const
{
    return x + width > other->x && 
            x < other->x + other->width && 
            y + height > other->y && 
            y < other->y + other->height;
}

bool Rectangle::willCollideX(const Rectangle* other, float xSpeed) const
{
    return x + width + xSpeed > other->x && 
            x + xSpeed < other->x + other->width && 
            y + height > other->y && 
            y < other->y + other->height;
}

bool Rectangle::willCollideY(const Rectangle* other, float ySpeed) const
{
    return x + width > other->x && 
            x < other->x + other->width && 
            y + height + ySpeed > other->y && 
            y + ySpeed < other->y + other->height;
}

bool Rectangle::willCollide(const Rectangle* other, float xSpeed, float ySpeed) const
{
    return x + width + xSpeed > other->x && 
            x + xSpeed < other->x + other->width && 
            y + height + ySpeed > other->y && 
            y + ySpeed < other->y + other->height;
}

float Rectangle::getDistanceX(const Rectangle* other) const
{
    if (x + width < other->x)
    {
        return other->x - (x + width);
    }
    else if (x > other->x + other->width)
    {
        return x - (other->x + other->width);
    }
    return 0.0f;
}

float Rectangle::getDistanceY(const Rectangle* other) const
{
    if (y + height < other->y)
    {
        return other->y - (y + height);
    }
    else if (y > other->y + other->height)
    {
        return y - (other->y + other->height);
    }
    return 0.0f;
}