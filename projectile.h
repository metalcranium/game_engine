#pragma once
#include "object.h"

class Projectile : public Object{
  public:
    Vector2 origin;
    Vector2 direction;
    float rotation;
    float angle;
    Rectangle destination;

    Projectile();
    ~Projectile();
};

