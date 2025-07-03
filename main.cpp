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

class World{
  public:
    std::vector<RigidBody2D>static_bodies;
    std::vector<RigidBody2D>dynamic_bodies;
};
class RigidBody2D{
  public: 
    Vector2 velocity;
    Vector2 position;
    Vector2 size;
    Rectangle collider;

    RigidBody2D(){
      std::cout << "rigid body created" << std::endl;
    }
    ~RigidBody2D(){
      std::cout << "rigid body destroyed" << std::endl;
    }
    virtual void update(){
      
    }
    virtual void draw(){
      
    }
};
class Block : public RigidBody2D{
  public:
    float fall;

    Block(){
      velocity = {0,0};
      size = {32,32};
      fall = gravity;
    }
    virtual void update(){
      position.y += velocity.y * fall * delta;
      velocity.y += 5 * delta;
    }
    virtual void draw(){
      DrawRectangle(position.x, position.y, size.x, size.y, YELLOW);
    }
};
class Player : public RigidBody2D{
  public:
    const float SPEED = 200;
    float speed;
    float fall;
    bool blocked;
    bool is_grounded;
    bool can_jump;

    Player(){
      velocity = {0,0};
      position = {600, 0};
      size = {32, 32};
      speed = SPEED;
      fall = gravity;
      collider = {position.x, position.y, size.x, size.y};
      blocked = false;
      is_grounded = false;
      can_jump = false;
      std::cout << "player created" << std::endl;
    }
    ~Player(){
      std::cout << "player destroyed" << std::endl;
    }

    virtual void update(){
      // Vector2Normalize(velocity);
      position.x += velocity.x * speed * delta;
      position.y += velocity.y * fall * delta;
      velocity.y += 5 * delta;

      // std::cout << "is grounded: " << is_grounded << std::endl;

      collider = {position.x, position.y, size.x, size.y};
      input();
    }
    virtual void draw(){
      DrawRectangle(position.x, position.y, size.x, size.y, BLUE);
    }
    void input(){
      if (IsKeyDown(KEY_LEFT)){
        velocity.x = -1;
      }
      else if (IsKeyDown(KEY_RIGHT)){
        velocity.x = 1;
      }
      else{
        velocity.x = 0;
      }
      if (IsKeyPressed(KEY_UP) and can_jump == true){
        velocity.y = -5;
        is_grounded = false;
      }
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
};

void Resolve_World_Collision(std::shared_ptr<Player>player, std::vector<Rectangle>boxes);

int main(){
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

  // std::unique_ptr<Player> player = std::make_unique<Player>();
  std::shared_ptr<Player>player = std::make_shared<Player>();
  std::vector<Rectangle>boxes;
  std::vector<std::shared_ptr<Block>>blocks;
  Rectangle collision;
  Rectangle ground = {0, 750, scr_width, 50};
  boxes.push_back(ground);
  
  while (!WindowShouldClose()){
    // update

    player->update();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
      std::unique_ptr<Rectangle>obj = std::make_unique<Rectangle>();
      *obj = {GetMousePosition().x, GetMousePosition().y, 32, 32};
      boxes.push_back(*obj);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
      std::shared_ptr<Block>block = std::make_shared<Block>();
      block->position = {GetMousePosition().x, GetMousePosition().y};
      block->size = {32,32};
      blocks.push_back(block);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
      for (int i = 0; i < boxes.size(); i++){
        if (CheckCollisionPointRec(GetMousePosition(), boxes[i])){
          boxes.erase(boxes.begin()+ i);
        }
      }
      for (int i = 0; i < blocks.size(); i++){
        if (CheckCollisionPointRec(GetMousePosition(), {blocks[i]->position.x,
                                    blocks[i]->position.y, blocks[i]->size.x, blocks[i]->size.y})){
          blocks.erase(blocks.begin()+ i);
        }
      }
    }

    for (auto i : blocks){
      i->update();
      std::cout << i->position.y << std::endl;
    }
    // runs the game from inside the editor in separate window
    if (IsKeyPressed(KEY_F5)){
      system("g++ test.cpp -o test -lraylib && ./test");
    }
    for (auto i : boxes){
      collision = GetCollisionRec(player->collider, i);
    }
    Resolve_World_Collision(player, boxes);


    // draw
    BeginDrawing();
    ClearBackground(BLACK);
    
    player->draw();
    for (auto j : boxes){
      DrawRectangleRec(j, RED);
    }
    for (auto i : blocks){
      i->draw();
    }
    DrawRectangleRec(collision, GREEN);

    EndDrawing();
    
  }
  CloseWindow();
  
}
void Resolve_World_Collision(std::shared_ptr<Player>player, std::vector<Rectangle>boxes){
  Vector2 sign = {0,0};
  bool collided;
  Rectangle collision;
  for (int i = 0; i < boxes.size(); i++){
    collided = CheckCollisionRecs(player->collider, boxes[i]);
    collision = GetCollisionRec(player->collider, boxes[i]);
    if (collided){//(collision.width != 0 and collision.height != 0){
      sign.x = player->collider.x + player->collider.width < boxes[i].x + boxes[i].width ? 1 : -1;
      sign.y = player->collider.y + player->collider.height < boxes[i].y + boxes[i].height ? 1 : -1;
        if (collision.width < collision.height){
          player->position.x -= collision.width * sign.x;
        }
        else if (collision.height < collision.width){
          // std::cout << "collision" << std::endl;
          player->is_grounded = true;
          player->position.y -= collision.height * sign.y;
          player->velocity.y = 0;
        }
        else{
          player->is_grounded = false;
        }
    }
  }
}
