#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <vector>
#include <iostream>

#define scr_width 1200
#define scr_height 800
#define delta GetFrameTime()
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
class Player : public RigidBody2D{
  public:
    const float SPEED = 200;
    float speed;
    bool blocked;

    Player(){
      velocity = {0,1};
      position = {600, 0};
      size = {32, 32};
      speed = SPEED;
      collider = {position.x, position.y, size.x, size.y};
      blocked = false;
      std::cout << "player created" << std::endl;
    }
    ~Player(){
      std::cout << "player destroyed" << std::endl;
    }

    void update(){
      Vector2Normalize(velocity);
      position.x += velocity.x * speed * delta;
      position.y += velocity.y * 100 * delta;

      collider = {position.x, position.y, size.x, size.y};
      input();
      if (blocked) {
        speed = 0;
      }
      else{
        speed = SPEED;
      }
    }
    void draw(){
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
      if (IsKeyPressed(KEY_UP)){
        velocity.y = -1;
      }
      else if (IsKeyDown(KEY_DOWN)){
        velocity.y = 1;
      }
      else{
        velocity.y += 1 * delta;
      }
      // else{
      //   velocity.y = 0;
      // }
      
    }
};

void Resolve_World_Collision(std::shared_ptr<Player>player, std::vector<Rectangle>boxes);

int main(){
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

  // std::unique_ptr<Player> player = std::make_unique<Player>();
  std::shared_ptr<Player>player = std::make_shared<Player>();
  std::vector<Rectangle>boxes;
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
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
      for (int i = 0; i < boxes.size(); i++){
        if (CheckCollisionPointRec(GetMousePosition(), boxes[i])){
          boxes.erase(boxes.begin()+ i);
        }
      }
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
    DrawRectangleRec(collision, GREEN);

    EndDrawing();
    
  }
  CloseWindow();
  
}
void Resolve_World_Collision(std::shared_ptr<Player>player, std::vector<Rectangle>boxes){
  bool collided = false;
  Rectangle collision;
  Vector2 sign = {0,0};
  std::cout << sign.x << "," << sign.y << std::endl;
    for (Rectangle i : boxes){
      collided = CheckCollisionRecs(player->collider, i);
      collision = GetCollisionRec(player->collider, i);

      if (collided){//(collision.width != 0 and collision.height != 0){
        sign.x = player->collider.x + player->collider.width < i.x + i.width ? 1 : -1;
        sign.y = player->collider.y + player->collider.height < i.y + i.height ? 1 : -1;
        std::cout << sign.x << sign.y << std::endl;
          // if (collision.height < player->size.y/8){
          //   player->position.y -= collision.height * sign.y;
          // }
          // if (collision.width < player->size.x/8){
          //   player->position.x -= collision.width * sign.x;
          // }
          if (collision.width < collision.height){
            player->position.x -= collision.width * sign.x;
          }
          else{
            player->position.y -= collision.height * sign.y;
          }
      }
    }
}
