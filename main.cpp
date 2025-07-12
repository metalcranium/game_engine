#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

#define delta GetFrameTime()
#define gravity 50
#define fps 60

class RigidBody2D;
class Player;
// mass, position, velocity, acceleration
// acceleratino = velocity * delta * mass
// force = mass * acceleration
// kinetic energy = .5 * mass * speed^2
class RigidBody2D{
  public: 
    Vector2 velocity;
    Vector2 position;
    Vector2 size;
    float mass;
    Rectangle collider;
    Color color;
    bool is_static;
    bool is_grounded;

    RigidBody2D(){
      std::cout << "rigid body created" << std::endl;
    }
    ~RigidBody2D(){
      std::cout << "rigid body destroyed" << std::endl;
    }
    virtual void update(){
      velocity.x -= velocity.x * delta * mass;
    }
    virtual void draw(){
      DrawRectangle(position.x, position.y, size.x, size.y, color);
    }
};
class Block : public RigidBody2D{
  public:
    float fall;
    float vel;

    Block(){
      velocity = {0,0};
      size = {32,32};
      fall = gravity;
    }
    virtual void update(){
      position.y += velocity.y * 200 * delta;
      collider = {position.x, position.y, size.x, size.y};

      if (is_static){
        velocity = {0,0};
      }
      else{
        velocity += {0,5 * delta};
      }
    }
    virtual void draw(){
      DrawRectangle(position.x, position.y, size.x, size.y, color);
    }
};
class Player : public RigidBody2D{
  public:
    const float SPEED = 200;
    float speed;
    float fall;

    float jump_origin = 0;
    float max_jump = 300;
    bool is_blocked;
    bool can_jump;

    Player(){
      velocity = {0,1};
      position = {600, 0};
      size = {32, 32};
      mass = 6;
      speed = SPEED;
      fall = gravity;
      collider = {position.x, position.y, size.x, size.y};
      is_blocked = false;
      is_grounded = false;
      can_jump = false;
      is_static = false;
      std::cout << "player created" << std::endl;
    }
    ~Player(){
      std::cout << "player destroyed" << std::endl;
    }

    virtual void update(){
      Vector2Normalize(velocity);
      position.x += velocity.x * speed * delta;
      position.y += velocity.y * speed * delta;
      std::cout << "is grounded: " << is_grounded << std::endl;

      collider = {position.x, position.y, size.x, size.y};
      input();
      if (is_blocked){
        bounce();
      }
    }
    virtual void draw(){
      DrawRectangle(position.x, position.y, size.x, size.y, BLUE);
    }
    void input(){
      if (IsKeyDown(KEY_LEFT)){
        move_left();
      }
      else if (IsKeyDown(KEY_RIGHT)){
        move_right();
      }
      else{
        velocity.x -= (velocity.x * delta)*mass;
      }
      if (IsKeyDown(KEY_UP) and can_jump){
          jump();
      }
      // if (IsKeyDown(KEY_UP)){
      //   velocity.y = -1;
      // }
      // else if (IsKeyDown(KEY_DOWN)){
      //   velocity.y = 1;
      // }
      // else{
      //   velocity.y = 0;
      // }
      if (is_grounded){
        can_jump = true;
      }
      else{
        can_jump = false;
        player_fall();
      }
    }
    void jump(){
        velocity.y = -3;
        is_grounded = false;
    }
    void player_fall(){
      can_jump = false;
      velocity.y += 5 * delta;
    }
    void move_left(){
      velocity.x = -1;
    }
    void move_right(){
      velocity.x = 1; 
    }
    void bounce(){
      mass = 5;
    }
};
class World{
  public:
    int grid_count;
    float grid_size;
    std::vector<std::shared_ptr<RigidBody2D>>objects;

    void Save_World(){
      std::ofstream file;
      file.open("map.txt");
      for (auto i : objects){
        file << i->position.x << " ";
        file << i->position.y << " ";
        file << i->size.x << " ";
        file << i->size.y << " ";
        file << i->is_static << " " ;
        file << "\n";
      }
      file.close();
    }
    void Load_World(){
      std::ifstream file;
      file.open("map.txt");
      float x;
      float y;
      float width;
      float height;
      bool solid;
      while (file >> x >> y >> width >> height){
        std::shared_ptr<RigidBody2D>obj = std::make_shared<RigidBody2D>();
        obj->position.x = x;
        obj->position.y = y;
        obj->size.x = width;
        obj->size.y = height;
        obj->is_static = solid;
        obj->color = RED;
        objects.push_back(obj);
      }
      file.close();
    }
};
class AnimationPlayer{
  int frame_speed;
  int frame_counter;
  int current_frame;
};

class Editor{
  
};
void Resolve_World_Collision(World world);
void Resolve_World_Collision(std::shared_ptr<Player>player, std::shared_ptr<World>world);
void Draw_Grid(World world);
void Game();
int main(){

  int scr_width = 1200;
  int scr_height = 800;
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

// TODO: Viewport zoom  
// TODO: mouse ?

  Camera2D camera;
  camera.offset = {(float(scr_width)/2), (float(scr_height)/2)};
  camera.target = {(float(scr_width)/2), (float(scr_height)/2)};
  camera.rotation = 0;
  camera.zoom = 1;

  float speed = 300;
  Vector2 velocity = {0,0};

  RenderTexture viewport = LoadRenderTexture(scr_width, scr_height);
  Rectangle screen_rect = {0,0,float(viewport.texture.width),-float(viewport.texture.height)};
  
  World world;
  world.grid_count = 100;
  world.grid_size = 32;
  world.Load_World();
  // std::shared_ptr<Player>player = std::make_shared<Player>();
  // std::shared_ptr<Block>ground = std::make_shared<Block>();
  // ground->vel = 0;
  // ground->color = RED;
  // ground->position = {0, float(scr_height)-50};
  // ground->size = {float(scr_width), 50};
  // ground->is_static = true;
  // world->objects.push_back(ground);
  // world->objects.push_back(player);
  
  while (!WindowShouldClose()){
    //update
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
    // Vector2 mouse = GetMousePosition();
    // camera.target += velocity * speed * delta;
    std::cout << int(mouse.x) / 32 << "," << int(mouse.y)/32 << std::endl;
    std::cout << mouse.x << "," << mouse.y << std::endl;
    std::cout << GetMousePosition().x << "," << GetMousePosition().y << std::endl;
    if (IsKeyDown(KEY_LEFT)){
      // velocity.x = -1;
      camera.target.x -= speed * delta;
    }
    else if (IsKeyDown(KEY_RIGHT)){
      // velocity.x = 1;
      camera.target.x += speed * delta;
    }
    // else{
    //   velocity.x = 0;
    // }
    if (IsKeyDown(KEY_UP)){
      // velocity.y = -1;
      camera.target.y -= speed * delta;
    }
    else if (IsKeyDown(KEY_DOWN)){
      // velocity.y = 1;
      camera.target.y += speed * delta;
    }
    // else{
    //   velocity.y = 0;
    // }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
      std::shared_ptr<Block>obj = std::make_shared<Block>();
      // obj->position = {GetMousePosition().x, GetMousePosition().y};
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x*32), float(position_y*32) };
      obj->size = {32, 32};
      obj->vel = 0;
      obj->color = RED;
      obj->is_static = true;
      world.objects.push_back(obj);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
      std::shared_ptr<Block>obj = std::make_shared<Block>();
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x*32), float(position_y*32) };
      obj->size = {32, 32};
      obj->vel = 0;
      obj->color = YELLOW;
      obj->is_static = false;
      world.objects.push_back(obj);
      // block->position = {GetMousePosition().x, GetMousePosition().y};
      // block->size = {32,32};
      // block->vel = 5;
      // block->color = YELLOW;
      // block->is_static = false;
      // world.objects.push_back(block);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
      for (int i = 0; i < world.objects.size(); i++){
        if (CheckCollisionPointRec(GetMousePosition(), world.objects[i]->collider)){
          world.objects.erase(world.objects.begin()+ i);
        }
      }
    }

    // runs the game from inside the editor in separate window
    if (IsKeyPressed(KEY_F5)){
      // system("g++ test.cpp -o test -lraylib && ./test");
      Game();
    }
    if (IsKeyPressed(KEY_F2)){
      world.Save_World();
    }
// viewport and UI
    BeginTextureMode(viewport);
    ClearBackground(GRAY);
    BeginMode2D(camera);


    for (auto i : world.objects){
      i->draw();
    }
    Draw_Grid(world);


    EndMode2D();
    EndTextureMode();
    
    // draw
    BeginDrawing();
    ClearBackground(BLACK);
    // player->draw();
    // for (auto i : world->objects){
    //   i->draw();
    // }


    DrawTextureRec(viewport.texture, screen_rect, {0,0}, WHITE);
    EndDrawing();
    
  }
  CloseWindow();
  
}
// Use this one for player due to the is_grounded flag
void Resolve_World_Collision(std::shared_ptr<Player>player, std::shared_ptr<World>world){
  Vector2 sign = {0,0};
  bool collided;
  Rectangle collision;
  for (auto i : world->objects){
    collided = CheckCollisionRecs(player->collider, i->collider);
    collision = GetCollisionRec(player->collider, i->collider);
    if (collided){
      sign.x = player->collider.x + player->collider.width < i->collider.x + i->collider.width ? 1 : -1;
      sign.y = player->collider.y + player->collider.height < i->collider.y + i->collider.height ? 1 : -1;
        if (collision.width < collision.height){
          player->position.x -= collision.width * sign.x;
          player->velocity.x *= -1;
          player->is_blocked = true;
          if (!i->is_static){
            i->position.x += collision.width * sign.x;// * (player->speed * delta);
            // i->position.x += (sign.x * delta)* player->speed * player->mass;
          }
          // break;
        }
        else if (collision.height < collision.width){
          player->is_grounded = true;
          player->position.y -= collision.height * sign.y;
          player->velocity.y = 0;

          if (!i->is_static){
            i->position.y += collision.height * sign.y;
          }
        }
        else{
          player->is_grounded = false;
          // break;
        }
    }
  }
}
void Resolve_World_Collision(World world){
  Vector2 sign = {0,0};
  bool collided;
  Rectangle collision;
  for (auto i : world.objects){
    for (auto j : world.objects){
      if (i == j){
        break;
      }
      collided = CheckCollisionRecs(i->collider, j->collider);
      collision = GetCollisionRec(i->collider, j->collider);
      if (collided){
        sign.x = i->collider.x + i->collider.width < j->collider.x + j->collider.width ? 1 : -1;
        sign.y = i->collider.y + i->collider.height < j->collider.y + j->collider.height ? 1 : -1;
        if (collision.width < collision.height){
          if (!i->is_static){
            i->position.x -= collision.width * sign.x;
            i->velocity.x = 0;
          }
          if (!j->is_static){
            j->position.x += collision.width * sign.x;
            j->velocity.x = 0;
          }
        }
        if (collision.height < collision.width){
          i->is_grounded = true;
          j->is_grounded = true;
          if (!i->is_static){
            i->position.y -= collision.height * sign.y;
            i->velocity.y = 0;
          }
          if (!j->is_static){
            j->position.y += collision.height * sign.y;
            j->velocity.y = 0;
          }
        }
      }
      else{
        i->is_grounded = false;
      }

    }
  }
}
void Draw_Grid(World world){
  
    for(float i = 0; i <= world.grid_count; i++){
      DrawLineV({0 * world.grid_size, i * world.grid_size}, {float(world.grid_count) * world.grid_size,i * world.grid_size}, DARKGRAY);
    }
    for(float i = 0; i <= world.grid_count; i++){
      DrawLineV({i * world.grid_size, 0* world.grid_size}, {i * world.grid_size, float(world.grid_count) * world.grid_size}, DARKGRAY);
    }
}
void Game(){
  int scr_width = 1200;
  int scr_height = 800;
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

// TODO: Viewport zoom  
// TODO: mouse ?

  
  World world;
  world.grid_count = 100;
  world.grid_size = 32;
  world.objects.clear();
  world.Load_World();
  std::shared_ptr<Player>player = std::make_shared<Player>();
  Rectangle collision;
  std::shared_ptr<Block>ground = std::make_shared<Block>();
  ground->vel = 0;
  ground->color = RED;
  ground->position = {0, float(scr_height)-50};
  ground->size = {float(scr_width), 50};
  ground->is_static = true;
  world.objects.push_back(ground);
  world.objects.push_back(player);
  
  while (!WindowShouldClose()){
    std::cout << "word size: " << world.objects.size() << std::endl;
    Vector2 mouse = GetMousePosition();
    // std::cout << int(mouse.x) / 32 << "," << int(mouse.y)/32 << std::endl;
    std::cout << mouse.x << "," << mouse.y << std::endl;
    // std::cout << GetMousePosition().x << "," << GetMousePosition().y << std::endl;

    // std::cout << world->objects.size() << std::endl;
    // update
    // world->Resolve_World_Collision();
    Resolve_World_Collision(world);
    // Resolve_World_Collision(player, world);
    // player->update();
    for (auto i : world.objects){
      i->update();
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
      std::shared_ptr<Block>obj = std::make_shared<Block>();
      // obj->position = {GetMousePosition().x, GetMousePosition().y};
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x*32), float(position_y*32) };
      obj->size = {32, 32};
      obj->vel = 0;
      obj->color = RED;
      obj->is_static = true;
      world.objects.push_back(obj);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
      std::shared_ptr<Block>block = std::make_shared<Block>();
      block->position = {GetMousePosition().x, GetMousePosition().y};
      block->size = {32,32};
      block->vel = 5;
      block->color = YELLOW;
      block->is_static = false;
      world.objects.push_back(block);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
      for (int i = 0; i < world.objects.size(); i++){
        if (CheckCollisionPointRec(GetMousePosition(), world.objects[i]->collider)){
          world.objects.erase(world.objects.begin()+ i);
        }
      }
    }

    
    // draw
    BeginDrawing();
    ClearBackground(BLACK);
    // player->draw();
    for (auto i : world.objects){
      i->draw();
    }
    EndDrawing();
    
  }
  CloseWindow();
  
}
