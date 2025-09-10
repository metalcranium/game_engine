#pragma once
#include "projectile.h"

class Arrow : public Projectile{
  public:
    Arrow();
    ~Arrow();
    void update();
    void draw();
    
};
