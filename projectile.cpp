#include "projectile.h"
#include <raymath.h>
#include <iostream>
#include "utility.h"
Projectile::Projectile(Vector2 mouse, Vector2 shooter){
  speed = 300;
  position = {shooter.x + 16, shooter.y};
  direction = {mouse.x, mouse.y};
  velocity = Vector2Normalize(direction - position);
  angle = Get_Angle(mouse, shooter);
  rotation = Direction_To(mouse, shooter);
  destination = {0+shooter.x+16, 0+shooter.y+16, size.x, size.y};
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

