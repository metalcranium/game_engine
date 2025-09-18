#include <fstream>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "object.h"
#include "player.h"

#define delta GetFrameTime()
#define gravity 200
#define fps 60

// mass, position, velocity, acceleration
// acceleration = velocity * delta * mass
// force = mass * acceleration
// kinetic energy = .5 * mass * speed^2

struct Tile {
  Vector2 position;
  Vector2 size;
  Rectangle source;
  Texture texture;
};
struct Atlas {
  Texture texture;
  int grid_size;
};
class World {
public:
  int grid_count;
  float grid_size;
  std::vector<std::shared_ptr<Object>> objects;
  std::vector<Tile> background;
  std::vector<Tile> foreground;

  void Update() {
    for (auto i : objects) {
      i->update();
    }
  }
  void Draw() {
    for (auto i : objects) {
      i->draw();
    }
  }
  void Save_World() {
    std::ofstream file;
    file.open("map.txt");
    for (auto i : objects) {
      file << i->position.x << " ";
      file << i->position.y << " ";
      file << i->size.x << " ";
      file << i->size.y << " ";
      file << i->source.x << " ";
      file << i->source.y << " ";
      file << i->is_static << " ";
      file << "\n";
    }
    file.close();
  }
  void Load_World(Texture &texture) {
    std::ifstream file;
    file.open("map.txt");
    float x;
    float y;
    float width;
    float height;
    float sourcex;
    float sourcey;
    bool solid;
    while (file >> x >> y >> width >> height >> sourcex >> sourcey >> solid) {
      std::shared_ptr<Object> obj = std::make_shared<Object>();
      obj->position.x = x;
      obj->position.y = y;
      obj->size.x = width;
      obj->size.y = height;
      obj->source = {sourcex, sourcey, 32, 32};
      obj->is_static = solid;
      obj->texture = texture;
      // obj->color = RED;
      objects.push_back(obj);
    }
    file.close();
  }
  void Print_World() {
    for (auto obj : objects) {
      std::cout << obj->position.x << " " << obj->position.y << " "
                << obj->size.x << " " << obj->size.y << " " << obj->is_static
                << std::endl;
      // std::cout << obj->rotation << std::endl;
    }
  }
  void Resolve_World_Collision() {
    Vector2 sign = {0, 0};
    bool collided;
    Rectangle collision;
    for (auto i : objects) {
      for (auto j : objects) {
        if (i == j) {
          break;
        }
        collided = CheckCollisionRecs(i->collider, j->collider);
        collision = GetCollisionRec(i->collider, j->collider);
        if (collided) {
          sign.x = i->collider.x + i->collider.width <
                           j->collider.x + j->collider.width
                       ? 1
                       : -1;
          sign.y = i->collider.y + i->collider.height <
                           j->collider.y + j->collider.height
                       ? 1
                       : -1;
          if (collision.width < collision.height) {
            // j->position.x += collision.width * sign.x;
          } else if (collision.height < collision.width) {
            // j->position.y += collision.height * sign.y;
          }
        }
      }
      i->is_grounded = false;
    }
  }
};
// Nearly pointless forward declaration
void Draw_Grid(World world);
void Game();
bool Is_Hovered(Rectangle body, Vector2 mouse) {
  bool hovered;
  if (mouse.x > body.x and mouse.x < body.x + body.width and
      mouse.y > body.y and mouse.y < body.y + body.height) {
    hovered = true;
  } else {
    hovered = false;
  }
  return hovered;
}

void Editor() {
  int scr_width = 1280;
  int scr_height = 720;
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

  Camera2D camera;
  camera.offset = {(float(scr_width) / 2), (float(scr_height) / 2)};
  camera.target = {(float(scr_width) / 2), (float(scr_height) / 2)};
  camera.rotation = 0;
  camera.zoom = 1;

  float speed = 300;
  Vector2 velocity = {0, 0};

  RenderTexture viewport = LoadRenderTexture(scr_width, scr_height);
  Rectangle screen_rect = {0, 0, float(viewport.texture.width),
                           -float(viewport.texture.height)};

  World world;
  world.grid_count = (scr_width / 32) + (scr_height / 32);
  world.grid_size = 32;

  std::cout << "count: " << world.objects.size() << std::endl;

  Atlas atlas;
  atlas.grid_size = 32;
  atlas.texture = LoadTexture("Assets/forestgroundtileset.png");
  world.Load_World(atlas.texture);

  Vector2 source;

  while (!WindowShouldClose()) {
    // update
    // std::cout << "tiles: " << world.objects.size() << std::endl; 
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
    // std::cout << mouse.x/32 << "," << mouse.y/32 << std::endl;
    world.Update();
    if (IsKeyDown(KEY_LEFT)) {
      camera.target.x -= speed * delta;
    } else if (IsKeyDown(KEY_RIGHT)) {
      camera.target.x += speed * delta;
    }
    if (IsKeyDown(KEY_UP)) {
      camera.target.y -= speed * delta;
    } else if (IsKeyDown(KEY_DOWN)) {
      camera.target.y += speed * delta;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) and
        GetMousePosition().x >= scr_width - 350) {
      // subtracted for offset of Atlas
      source = {float(GetMouseX() / 32 - 29) * 32,
                float(GetMouseY() / 32) * 32};
      std::cout << "source: " << source.x << "," << source.y << std::endl;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) and
        GetMousePosition().x < scr_width - 350) {
      std::shared_ptr<Object> obj = std::make_shared<Object>();
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x * 32), float(position_y * 32)};
      std::cout << obj->position.x << "," << obj->position.y << std::endl;
      obj->size = {32, 32};
      // obj->color = RED;
      obj->source = {source.x, source.y, 32, 32};
      obj->texture = atlas.texture;
      obj->is_static = true;
      world.objects.push_back(obj);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
      std::shared_ptr<Object> obj = std::make_shared<Object>();
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x * 32), float(position_y * 32)};
      obj->size = {32, 32};
      // obj->color = YELLOW;
      obj->is_static = false;
      world.objects.push_back(obj);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      int pointx = int(mouse.x);
      int pointy = int(mouse.y);
      std::cout << pointx << " " << pointy << std::endl;
      for (int i = 0; i < world.objects.size(); i++) {
        Object obj = *world.objects[i];
        if (mouse.x > obj.position.x and
            mouse.x < obj.position.x + obj.size.x and
            mouse.y > obj.position.y and
            mouse.y < obj.position.y + obj.size.y) {
            world.objects.erase(world.objects.begin() + i);
        }
      }
    }
    // runs the game from inside the editor in separate window
    if (IsKeyPressed(KEY_F5)) {
      // system("g++ test.cpp -o test -lraylib && ./test");
      Game();
    }
    if (IsKeyPressed(KEY_F2)) {
      world.Save_World();
    }
    // viewport and UI
    BeginTextureMode(viewport);
    ClearBackground(GRAY);
    BeginMode2D(camera);

    // world.Draw();
    for (auto i : world.objects) {
      i->draw();
    }
    Draw_Grid(world);

    EndMode2D();
    EndTextureMode();

    // draw
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawTextureRec(viewport.texture, screen_rect, {0, 0}, WHITE);
    DrawRectangle(scr_width - 350, 0, 350, scr_height, DARKGRAY);
    DrawTexture(atlas.texture, scr_width - 350, 0, WHITE);
    EndDrawing();
  }
  CloseWindow();
}
void Draw_Grid(World world) {
  for (float i = 0; i <= world.grid_count; i++) {
    DrawLineV({0 * world.grid_size, i * world.grid_size},
              {float(world.grid_count) * world.grid_size, i * world.grid_size},
              DARKGRAY);
  }
  for (float i = 0; i <= world.grid_count; i++) {
    DrawLineV({i * world.grid_size, 0 * world.grid_size},
              {i * world.grid_size, float(world.grid_count) * world.grid_size},
              DARKGRAY);
  }
}
void Game() {
  int scr_width = 1200;
  int scr_height = 600;
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

  World world;
  world.grid_count = 25;
  world.grid_size = 32;
  Rectangle collision;
  Atlas atlas;
  atlas.texture = LoadTexture("Assets/forestgroundtileset.png");
  world.Load_World(atlas.texture);
  std::shared_ptr<Player> player = std::make_shared<Player>();
  world.objects.push_back(player);


  while (!WindowShouldClose()) {
    Vector2 mouse = GetMousePosition();
    // float angle = std::atan2(mouse.y - player->position.y, mouse.x - player->position.x);
    // float rotation = angle * (180/M_PI) + 180 ;
    // std::cout << "angle: " << rotation << std::endl;

    // update
    world.Resolve_World_Collision();
    world.Update();
    if (IsKeyPressed(KEY_F8)) {
      Editor();
    }
    // draw
    BeginDrawing();
    ClearBackground(DARKGRAY);
    world.Draw();
    EndDrawing();
  }
  CloseWindow();
}
int main(){
  int choice;
  std::cout << "(1) for editor\n";
  std::cout << "(2) for game\n";
  std::cout << "(0) to exit\n";
  std::cout << "choice: ";
  std::cin >> choice;

  if (choice == 1){
    Editor();
  }
  else if (choice == 2){
    Game();
  }
  else if (choice == 0){
    return 0;
  }
  else {
    std::cout << "invalid input. please try again!" << std::endl;
    main();
  }
  // this is only need for running the cosmic desktop beause the issues that
  // cosmic has with glfw windows
  // system("cosmic-comp");
}
