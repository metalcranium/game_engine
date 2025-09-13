#include "projectile.h"
#include <raymath.h>
#include <iostream>
#include "utility.h"
Projectile::Projectile(Vector2 mouse, Vector2 object){
  speed = 300;
  position = {object.x + 16, object.y};
  direction = {mouse.x, mouse.y};
  velocity = Vector2Normalize(direction - position);
  // angle = atan2(mouse.y - object.y, mouse.x - object.x);
  angle = Get_Angle(mouse, object);
  // rotation = angle * (180/M_PI) + 180;
  rotation = Direction_To(mouse, object);
  destination = {0+object.x+16, 0+object.y+16, size.x, size.y};
  origin = {size.x/2, size.y/2};
  texture = LoadTexture("Assets/arrow.png");
  source = {0,0,32,32};

  std::cout << position.x << "," << position.y << std::endl;

  // TODO: need to impliment how to add the projectile
  // to the objects list in the world.
}
Projectile::~Projectile(){
  // std::cout << "projectile destroyed" << std::endl;
}
void Projectile::update(){
  rotation = angle * (180/M_PI) + 180;
  velocity = Vector2Normalize(direction - position);
  destination.x += velocity.x * speed * GetFrameTime();
  destination.y += velocity.y * speed * GetFrameTime();
  std::cout << position.x << "," << position.y << std::endl;
}
void Projectile::draw(){
  DrawTexturePro(texture, source, destination, origin, rotation, WHITE);
}

