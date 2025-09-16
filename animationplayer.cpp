#include "animationplayer.h"
#include <iostream>

AnimationPlayer::AnimationPlayer() {
	current_frame = 0;
}
AnimationPlayer::~AnimationPlayer() {
	std::cout << "animation deleted" << std::endl;
}
float AnimationPlayer::animate(Rectangle &source) {
	// frame_speed = fr_speed;
	// frames = frs;
  frame_counter++;
	if (frame_counter >= 60 / frame_speed){
		frame_counter = 0;
		current_frame++;

		source.x = float(current_frame) * source.width;
		if (current_frame >= frames){
			current_frame = 0;
		}
	}
	std::cout << "source x: " << source.x << std::endl;
  return source.x;
}
void AnimationPlayer::update(int fr_speed, int frs) {
	frame_speed = fr_speed;
	frames = frs;
}
