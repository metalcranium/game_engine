#pragma once
#include "raylib.h"


class Item{
  public:
    Vector2 position;
    Vector2 size; 
    int count;
    int max_stack;
    Texture texture;

    Item();
    ~Item();

    void use();
};
