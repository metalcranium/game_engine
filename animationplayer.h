#pragma once
#include "raylib.h"

struct AnimationPlayer {
  int frame_speed;
  int frame_counter;
  int current_frame;
  int frames;

  AnimationPlayer();
  ~AnimationPlayer();

  float animate(Rectangle &source, int fr_speed, int frs);
  void update(int fr_speed, int frs);
};
