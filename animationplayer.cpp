#include "animationplayer.h"

AnimationPlayer::AnimationPlayer() {}
AnimationPlayer::~AnimationPlayer() {}
float AnimationPlayer::animate(Rectangle &source) {
  frame_counter++;
	if (frame_counter >= 60 / frame_speed){
		frame_counter = 0;
		current_frame++;
		if (current_frame > frames){
			source.x = float(current_frame) * 32;
		}
	}
  return source.x;
}
void AnimationPlayer::update(int fr_speed, int frs) {
	frame_speed = fr_speed;
	frames = frs;
}
	
