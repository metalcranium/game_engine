#include "arrow.h"
#include "projectile.h"

Arrow::Arrow(){
  texture = LoadTexture("Assets/arrow.png");
  source = {0,0,32,32};
  speed = 300;
}
Arrow::~Arrow(){
  
}
void Arrow::update(){

}
void Arrow::draw(){
  
}
