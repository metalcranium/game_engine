#pragma once
#include "object.h"
#include "animationplayer.h"
#include <memory>

class Character : public Object{
  public:
    int health;
    int max_health;
    int mana;
    int max_mana;
    int damage;
    int armor;
    Texture weapon_socket;
    // AnimationPlayer* animation;
    std::unique_ptr<AnimationPlayer>animation;

    Character();
    ~Character();
    void ready() override;
    void update() override;
    void draw() override;
};
