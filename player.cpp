#include "player.h"
#include <iostream>

// TODO:
//for some reason the drawing of the player is screwed up.

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
  animation = std::make_unique<AnimationPlayer>();
  animation->update(10, 4);
}
void Player::update(){
    Vector2Normalize(velocity);
    position.x += velocity.x * speed * delta;
    position.y += velocity.y * speed * delta;
    // std::cout << "is grounded: " << is_grounded << std::endl;
    collider = {position.x, position.y, size.x, size.y};
    input();
    animation->animate(source);
    // animation->update(10, 6);
    for (auto i : projectiles){
      i->update();
    }
  
}
void Player::draw(){
  DrawTextureRec(texture, source, position, WHITE);
  if (projectiles.size() > 0){
    draw_projectiles();
  }
}
void Player::input(){
  Vector2 mouse = GetMousePosition();
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
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
    std::shared_ptr<Projectile>arrow = std::make_shared<Projectile>(mouse,position) ;
    arrow->texture = LoadTexture("Assets/fireball.png");
    projectiles.push_back(arrow);
    std::cout << projectiles.size() << std::endl;
  }
}
void Player::draw_projectiles(){
  for (auto i : projectiles){
    i->draw();
  }
}
