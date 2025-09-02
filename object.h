#pragma once
#include "raylib.h"

class Object {
		public:
			Vector2 velocity;
			Vector2 position;
			Vector2 size;
			Rectangle collider;
			Color color;
			Rectangle source;
			bool is_static;
			bool is_grounded;
			float speed;
			float mass;
			float fall;
			Texture atlas_texture;
			Texture idle_texture;
			Texture texture;

			Object();
			~Object();
			virtual void ready();
			virtual void update();
			virtual void draw();
};

