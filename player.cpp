#include "player.h"
#include "animationplayer.h"
#include <memory>
#include <raylib.h>
#include <raymath.h>

#define delta GetFrameTime()
Player::Player(){
  ready();
}
Player::~Player(){
  // delete animation;
}void Player::ready(){
  velocity = {0,0};
  position = {0,0};
  size = {32,32};
  speed = 200;
  atlas_texture = LoadTexture("Assets/herowalk.png");
  idle_texture = LoadTexture("Assets/hero.png");
  texture = idle_texture;
  source = {0,0, size.x, size.y};
  // animation = new(AnimationPlayer);
  animation = std::make_unique<AnimationPlayer>();
}
void Player::update(){
    Vector2Normalize(velocity);
    position.x += velocity.x * speed * delta;
    position.y += velocity.y * speed * delta;
    // std::cout << "is grounded: " << is_grounded << std::endl;
    collider = {position.x, position.y, size.x, size.y};
    input();
    animation->animate(source);
  
}
void Player::draw(){
  DrawTextureRec(texture, source, position, WHITE);
}
void Player::input(){
  if (IsKeyDown(KEY_UP)){
    texture = atlas_texture;
    velocity.y = -1;
  }
  else if (IsKeyDown(KEY_DOWN)){
    texture = atlas_texture;
    velocity.y = 1;
  }
  else {
    texture = idle_texture;
    velocity.y = 0;
  }
  if (IsKeyDown(KEY_LEFT)){
    texture = atlas_texture;
    source.width = size.x;
    velocity.x = -1;
  }
  else if (IsKeyDown(KEY_RIGHT)){
    texture = atlas_texture;
    source.width = -size.x;
    velocity.x = 1;
  }
  else {
    texture = idle_texture;
    velocity.x = 0;
  }
}
