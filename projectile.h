#pragma once
#include "object.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

class Projectile : public Object{
  public:
    Vector2 origin;
    Vector2 direction;
    float rotation;
    float angle;
    Rectangle destination;

    Projectile(Vector2 mouse_position, Object object);
    ~Projectile();
    void update();
    void draw();
};

