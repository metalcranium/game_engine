#pragma once
#include "raylib.h"


class Item{
  public:
    Vector2 position;
    Vector2 size; 
    int count;
    int max_stack;
    Rectangle pickup_range;
    Texture texture;
    bool is_on_ground;

    Item();
    ~Item();

    void draw();
    virtual void use();
};
