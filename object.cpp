#include "object.h"

Object::Object(){
	ready();
}
Object::~Object(){}
void Object::ready(){
	velocity = {0,0};
	size = {32,32};
}
void Object::update(){
	position.x += velocity.x * speed * GetFrameTime();
	position.y += velocity.y * speed * GetFrameTime();
	collider = {position.x, position.y, size.x, size.y};

	if (is_static){
		velocity = {0,0};
	}else{
		velocity = {0, 5 * GetFrameTime()};
	}
}
void Object::draw(){
	DrawTextureRec(texture, source, position, WHITE);
}
