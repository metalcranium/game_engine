#pragma once
#include "character.h"
// #include "item.h"
#include <memory>
#include <vector>
#include "animationplayer.h"
#include "raylib.h"
#include "raymath.h"
#include "projectile.h"

class Player : public Character{
  public:
    // std::vector<Item>inventory;
    // std::vector<Item>equipped;
    // std::vector<Item>tool_bar;
    std::vector<std::shared_ptr<Projectile>>projectiles;
    Player();
    ~Player();
    void ready() override;
    void update() override;
    void draw() override;
    void input();
};
