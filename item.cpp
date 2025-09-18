#include "item.h"

Item::Item(){
  
}
Item::~Item(){
  
}
void Item::draw(){
  DrawTexture(texture, position.x, position.y, WHITE);
}
void Item::use(){
  
}
