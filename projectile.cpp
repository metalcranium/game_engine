#include "projectile.h"
#include <raymath.h>
#include <iostream>
#include "utility.h"
Projectile::Projectile(Vector2 mouse, Vector2 shooter){
  speed = 300;
  position = {shooter.x + 16, shooter.y};
  // Direction is used to normalize the velocity of the projectiles travel.
  direction = {mouse.x, mouse.y};
  // velocity = Vector2Normalize(direction - position);
  // Having a seperate variable for the angle may not be necessary.
  // but i'm leaving it in for now.
  // angle = Get_Angle(mouse, shooter);
  rotation = Direction_To(mouse, shooter);
  // Where we want the sprite to be drawn.
  // Which is is the center of the player.
  destination = {shooter.x+16, shooter.y+16, size.x, size.y};
  // The point where the projectile rotates.
  // In this case is the center of the projctile.
  origin = {size.x/2, size.y/2};
  texture = LoadTexture("Assets/arrow.png");
  source = {0,0,32,32};

  std::cout << position.x << "," << position.y << std::endl;

  // TODO: need to impliment how to add the projectile
  // to the objects list in the world.
  // for now I have it being added to a list of projectiles
  // that is attached to the player.
}
Projectile::~Projectile(){
  // std::cout << "projectile destroyed" << std::endl;
}
void Projectile::update(){
  // rotation = angle * (180/M_PI) + 180;
  velocity = Vector2Normalize(direction - position);
  destination.x += velocity.x * speed * GetFrameTime();
  destination.y += velocity.y * speed * GetFrameTime();
  std::cout << position.x << "," << position.y << std::endl;
}
void Projectile::draw(){
  DrawTexturePro(texture, source, destination, origin, rotation, WHITE);
}

