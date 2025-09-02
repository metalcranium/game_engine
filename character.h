#pragma once
#include "object.h"

class Character : public Object{
  public:
    int health;
    int max_health;
    int mana;
    int max_mana;
    int damage;
    int armor;

    Character();
    ~Character();
    void ready() override;
    void update() override;
    void draw() override;
};
