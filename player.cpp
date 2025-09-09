#include "player.h"
#include "animationplayer.h"
#include <raylib.h>
#include <raymath.h>

#define delta GetFrameTime()
Player::Player(){
  ready();
}
Player::~Player(){
  delete animation;
}void Player::ready(){
  velocity = {0,0};
  position = {0,0};
  size = {32,32};
  speed = 200;
  atlas_texture = LoadTexture("Assets/herowalk.png");
  idle_texture = LoadTexture("Assets/hero.png");
  texture = idle_texture;
  source = {0,0, size.x, size.y};
  animation = new(AnimationPlayer);
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
    velocity.y = -1;
  }
  else if (IsKeyDown(KEY_DOWN)){
    velocity.y = 1;
  }
  else {
    velocity.y = 0;
  }
  if (IsKeyDown(KEY_LEFT)){
    velocity.x = -1;
  }
  else if (IsKeyDown(KEY_RIGHT)){
    velocity.x = 1;
  }
  else {
    velocity.x = 0;
  }
}
