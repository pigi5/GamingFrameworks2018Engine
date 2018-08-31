#include "../header/Rectangle.h"

Rectangle::Rectangle(float x, float y, float width, float height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

// Tests if this rectangle currently overlaps the given rectangle.
// params:
//   other - rectangle to test collision against
// returns: whether this rectangle overlaps the given rectangle
bool Rectangle::collides(const Rectangle* other) const
{
    return x + width > other->x && 
            x < other->x + other->width && 
            y + height > other->y && 
            y < other->y + other->height;
}

// Tests if this rectangle will collide with the given rectangle in the x-axis after its new 
// position is calculated based on its speed.
// params:
//   other - rectangle to test collision against
//   xSpeed - offset in the x-axis
// returns: whether this rectangle will collide with the given rectangle
bool Rectangle::willCollideX(const Rectangle* other, float xSpeed) const
{
    return x + width + xSpeed > other->x && 
            x + xSpeed < other->x + other->width && 
            y + height > other->y && 
            y < other->y + other->height;
}

// Tests if this rectangle will collide with the given rectangle in the y-axis after its new 
// position is calculated based on its speed.
// params:
//   other - rectangle to test collision against
//   ySpeed - offset in the y-axis
// returns: whether this rectangle will collide with the given rectangle
bool Rectangle::willCollideY(const Rectangle* other, float ySpeed) const
{
    return x + width > other->x && 
            x < other->x + other->width && 
            y + height + ySpeed > other->y && 
            y + ySpeed < other->y + other->height;
}

// Tests if this rectangle will collide with the given rectangle after its new position is
// calculated based on its speed.
// params:
//   other - rectangle to test collision against
//   xSpeed - offset in the x-axis
//   ySpeed - offset in the y-axis
// returns: whether this rectangle will collide with the given rectangle
bool Rectangle::willCollide(const Rectangle* other, float xSpeed, float ySpeed) const
{
    return x + width + xSpeed > other->x && 
            x + xSpeed < other->x + other->width && 
            y + height + ySpeed > other->y && 
            y + ySpeed < other->y + other->height;
}

// Calculate the distance along the x-axis to the given rectangle.
// params:
//   other - rectangle to test distance against
// returns: x-axis distance to the given object
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

// Calculate the distance along the y-axis to the given rectangle.
// params:
//   other - rectangle to test distance against
// returns: y-axis distance to the given object
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