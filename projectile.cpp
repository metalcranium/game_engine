#include "projectile.h"
#include <raymath.h>
Projectile::Projectile(Vector2 mouse, Object object){
  position = {object.position.x + 16, object.position.y};
  direction = {mouse.y, mouse.y};
  velocity = Vector2Normalize(direction - position);
  angle = atan2(mouse.y - object.position.y, mouse.x - object.position.x);
  rotation = angle * (180/M_PI) + 180;
  destination = {object.position.x+16, object.position.y+16};
  origin = {size.x/2, size.y/2};

  // TODO: need to impliment how to add the projectile
  // to the objects list in the world.
}
Projectile::~Projectile(){
  
}
void Projectile::update(){
  // rotation = angle * (180/M_PI) + 180;
  // velocity = Vector2Normalize(direction - position);
  destination.x += velocity.x * speed * GetFrameTime();
  destination.y += velocity.y * speed * GetFrameTime();
}
void Projectile::draw(){
  DrawTexturePro(texture, source, destination, origin, rotation, WHITE);
}

