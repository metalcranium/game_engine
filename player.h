#pragma once
#include "character.h"
#include "item.h"
#include <memory>
#include <vector>
class Player : Character{
  public:
    std::vector<Item>inventory;
    std::vector<Item>equipt;
    std::vector<Item>tool_bar;
    Player();
    ~Player();
    void ready() override;
    void update() override;
    void draw() override;
    void input();
};
