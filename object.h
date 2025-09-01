#pragma once
#include "raylib.h"

class Object {
		public:
			Vector2 velocity;
			Vector2 position;
			Vector2 size;
			Rectangle collider;
			Rectangle source;
			float speed;
			float mass;
			float fall;
			Texture atlas;
			Texture texture;
			bool is_static;
			bool is_grounded;

			Object();
			~Object();
			virtual void ready();
			virtual void update();
			virtual void draw();
};

