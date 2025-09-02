#include "player.h"
#include <raylib.h>

Player::Player(){
  ready();
}
Player::~Player(){
  
}void Player::ready(){
  velocity = {0,0};
  position = {0,0};
  size = {32,32};
  speed = 200;
  atlas = LoadTexture("Assets/herowalk.png");
  texture = LoadTexture("Assets/hero.png");
  source = {0,0, size.x, size.y};
}
void Player::update(){
  
}
void Player::input(){
  if (IsKeyDown(KEY_UP)){
    velocity.y = -1;
  }
  else if (IsKeyDown(KEY_DOWN)){
    velocity.y = 1;
  }
  if (IsKeyDown(KEY_LEFT)){
    velocity.x = -1;
  }
  else if (IsKeyDown(KEY_RIGHT)){
    velocity.x = 1;
  }
}
