#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <vector>
#include <iostream>

#define scr_width 1200
#define scr_height 800
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
    bool is_blocked;
    bool is_grounded;
    bool can_jump;

    Player(){
      velocity = {0,0};
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
      velocity.y += 5 * delta;

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
      if (IsKeyPressed(KEY_UP) and can_jump == true){
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
      }
    }
    void jump(){
      velocity.y = -3;
      is_grounded = false;
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
    std::vector<std::shared_ptr<RigidBody2D>>objects;
};
class AnimationPlayer{
  int frame_speed;
  int frame_counter;
  int current_frame;
};

class Editor{
  
};
void Resolve_World_Collision(std::shared_ptr<World>world);
void Resolve_World_Collision(std::shared_ptr<Player>player, std::shared_ptr<World>world);

int main(){
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

// TODO: Viewport zoom  
// TODO: mouse ?

  Camera2D camera;
  camera.offset = {float(scr_width)/2, float(scr_height)/2};
  camera.target = {float(scr_width)/2 - 500, float(scr_height)/2 - 300};
  camera.rotation = 0;
  camera.zoom = 2;

  float speed = 50;
  Vector2 velocity = {0,0};

  RenderTexture viewport = LoadRenderTexture(scr_width - 400, scr_height);
  Rectangle screen_rect = {0,0,float(viewport.texture.width),float(viewport.texture.width)};
  
  std::shared_ptr<World>world = std::make_shared<World>();
  std::shared_ptr<Player>player = std::make_shared<Player>();
  Rectangle collision;
  std::shared_ptr<Block>ground = std::make_shared<Block>();
  ground->vel = 0;
  ground->color = RED;
  ground->position = {0, scr_height-50};
  ground->size = {scr_width, 50};
  ground->is_static = true;
  world->objects.push_back(ground);
  // world->objects.push_back(player);
  
  while (!WindowShouldClose()){
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
    camera.target += velocity * speed * delta;

    // std::cout << world->objects.size() << std::endl;
    // update
    // world->Resolve_World_Collision();
    Resolve_World_Collision(world);
    Resolve_World_Collision(player, world);
    player->update();
    for (auto i : world->objects){
      i->update();
    }
    if (IsKeyDown(KEY_LEFT)){
      velocity.x = -1;
    }
    else if (IsKeyDown(KEY_RIGHT)){
      velocity.x = 1;
    }
    else{
      velocity.x = 0;
    }
    if (IsKeyDown(KEY_UP)){
      velocity.y = -1;
    }
    else if (IsKeyDown(KEY_DOWN)){
      velocity.y = 1;
    }
    else{
      velocity.y = 0;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
      std::shared_ptr<Block>obj = std::make_shared<Block>();
      obj->position = {GetMousePosition().x, GetMousePosition().y};
      obj->size = {32, 32};
      obj->vel = 0;
      obj->color = RED;
      obj->is_static = true;
      world->objects.push_back(obj);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
      std::shared_ptr<Block>block = std::make_shared<Block>();
      block->position = {GetMousePosition().x, GetMousePosition().y};
      block->size = {32,32};
      block->vel = 5;
      block->color = YELLOW;
      block->is_static = false;
      world->objects.push_back(block);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
      for (int i = 0; i < world->objects.size(); i++){
        if (CheckCollisionPointRec(GetMousePosition(), world->objects[i]->collider)){
          world->objects.erase(world->objects.begin()+ i);
        }
      }
    }

    // runs the game from inside the editor in separate window
    if (IsKeyPressed(KEY_F5)){
      system("g++ test.cpp -o test -lraylib && ./test");
    }
// viewport and UI
    BeginTextureMode(viewport);
    ClearBackground(GRAY);
    BeginMode2D(camera);

    for(float i = 0; i <= 100; i++){
      DrawLineV({0, i * 32}, {100 * 32,i * 32}, DARKGRAY);
    }
    for(float i = 0; i <= 100; i++){
      DrawLineV({i * 32, 0}, {i * 32, 100 * 32}, DARKGRAY);
    }

    EndMode2D();
    EndTextureMode();
    
    // draw
    BeginDrawing();
    ClearBackground(BLACK);
    player->draw();
    for (auto i : world->objects){
      i->draw();
    }

    DrawRectangleRec(collision, GREEN);

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
void Resolve_World_Collision(std::shared_ptr<World>world){
  Vector2 sign = {0,0};
  bool collided;
  Rectangle collision;
  for (auto i : world->objects){
    for (auto j : world->objects){
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
          }
          if (!j->is_static){
            j->position.x += collision.width * sign.x;
          }
        }
        else if (collision.height < collision.width){
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
    }
  }
}
