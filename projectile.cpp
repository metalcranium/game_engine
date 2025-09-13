#include "projectile.h"
#include <raymath.h>
#include <iostream>
#include "utility.h"
Projectile::Projectile(Vector2 mouse, Vector2 shooter){
  speed = 300;
  position = {shooter.x + 16, shooter.y};
  // Direction is used to normalize the velocity of the projectiles travel.
  // But normalizeing the velocity may be suitable.
  // direction = {mouse.x, mouse.y};
  velocity = Vector2Normalize(mouse - Vector2{shooter.x + 16, shooter.y + 16});
  // Having a seperate variable for the angle may not be necessary.
  // but i'm leaving it in and commented for now.
  // angle = Get_Angle(mouse, shooter);
  rotation = Direction_To(mouse, shooter);
  // Where we want the sprite to be drawn.
  // Which is is the center of the player.
  destination = {shooter.x+16, shooter.y+16, size.x, size.y};
  // The point where the projectile rotates.
  // In this case is the center of the projctile.
  origin = {size.x/2, size.y/2};
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
  // velocity = Vector2Normalize(direction - position);
  destination.x += velocity.x * speed * GetFrameTime();
  destination.y += velocity.y * speed * GetFrameTime();
  std::cout << position.x << "," << position.y << std::endl;
}
void Projectile::draw(){
  DrawTexturePro(texture, source, destination, origin, rotation, WHITE);
}

