#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

#define delta GetFrameTime()
#define gravity 200
#define fps 60

// mass, position, velocity, acceleration
// acceleration = velocity * delta * mass
// force = mass * acceleration
// kinetic energy = .5 * mass * speed^2

struct AnimationPlayer{
 
  int frame_speed;
  int frame_counter;
  int current_frame;
  int frames;

  AnimationPlayer(){};
  ~AnimationPlayer(){
    std::cout << "animation deleted" << std::endl;
  }
  float animate(Rectangle &source){
    float source_x;
    std::cout << frame_speed << std::endl;
    std::cout << frame_counter << std::endl;
    std::cout << current_frame << std::endl;
    std::cout << frames << std::endl;
    frame_counter++;
    if (frame_counter >= fps/frame_speed){
      frame_counter = 0;
      current_frame++;
      if (current_frame > frames){
        source.x = float(current_frame) * 32;
      }
    }
    return source.x;
  }
  void update(int fr_speed, int frs){
    frame_speed = fr_speed;
    frames = frs;
  }
};
class RigidBody2D{
  public:
    Vector2 velocity;
    Vector2 position;
    Vector2 size;
    Rectangle collider;
    Color color;
    bool is_static;
    bool is_grounded;
    float mass;
    float speed;
    float fall;
    Rectangle source;
    Texture atlas_texture;
    Texture texture;
    Texture idle_texture;

    RigidBody2D(){
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
      DrawTextureRec(texture, source, position, WHITE);
    }
};
class Player : public RigidBody2D{
  public:
    const float SPEED = 100;
    Vector2 speed;
    float fall;
    bool can_jump;
    AnimationPlayer* animation;
    int frame_counter;
    int current_frame;
    int frame_speed;
    int frames;

    Player(){
      velocity = {0,1};
      position = {600, 0};
      size = {32, 32};
      mass = 6;
      fall = gravity;
      speed = {SPEED, fall};
      collider = {position.x, position.y, size.x, size.y};
      is_grounded = false;
      can_jump = false;
      is_static = false;
      source = {0,0,32,32};
      atlas_texture = LoadTexture("herowalk.png");
      idle_texture = LoadTexture("hero.png");
<<<<<<< HEAD
      animation = new(AnimationPlayer);
      animation->frame_counter = 0;
      animation->current_frame = 0;
      animation->frame_speed = 10;
=======
      frame_counter = 0;
      current_frame = 0;
      frame_speed = 10;
      frames = 5;
>>>>>>> 2eee2f33cc2093715c8e9344ae8a2c05502c5221
      std::cout << "player created" << std::endl;
    }
    ~Player(){
      std::cout << "player destroyed" << std::endl;
    }
    virtual void update(){
      Vector2Normalize(velocity);
      position.x += velocity.x * speed.x * delta;
      position.y += velocity.y * speed.y * delta;
      // std::cout << "is grounded: " << is_grounded << std::endl;
      collider = {position.x, position.y, size.x, size.y};
      input();
<<<<<<< HEAD
      animation->animate(source);   

      
=======

      frame_counter++;
      if (frame_counter >= fps/frame_speed){
        frame_counter = 0;
        current_frame++;
        if (current_frame > frames){
          source.x = float(current_frame) * 32;
        }
      }
>>>>>>> 2eee2f33cc2093715c8e9344ae8a2c05502c5221
    }
    virtual void draw(){
      DrawTextureRec(texture, source, position, WHITE);
    }
    void input(){
      if (IsKeyDown(KEY_LEFT)){
        move_left();
      }
      else if (IsKeyDown(KEY_RIGHT)){
        move_right();
      }
      else{
        idle();
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
        velocity.y = 0;
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
      texture = atlas_texture;
      source.width = size.x;
      velocity.x = -1;
    }
    void move_right(){
      texture = atlas_texture;
      source.width = -size.x;
      velocity.x = 1;
    }
    void idle(){
      texture = idle_texture;
      velocity.x -= (velocity.x * delta)*mass;
    }
    void bounce(){
      mass = 5;
    }
};
struct Tile{
  Vector2 position;
  Vector2 size;
  Rectangle source;
  Texture texture;
};
struct Atlas{
  Texture texture;
  int grid_size;
};
class World{
  public:
    int grid_count;
    float grid_size;
    std::vector<std::shared_ptr<RigidBody2D>>objects;
    std::vector<Tile>background;
    std::vector<Tile>foreground;

    void Update(){
      for (auto i : objects){
        i->update();
      }
    }
    void Draw(){
      for (auto i : objects){
        i->draw();
      }
    }
    void Save_World(){
      std::ofstream file;
      file.open("map.txt");
      for (auto i : objects){
        file << i->position.x << " ";
        file << i->position.y << " ";
        file << i->size.x << " ";
        file << i->size.y << " ";
        file << i->source.x << " ";
        file << i->source.y << " ";
        file << i->is_static << " " ;
        file << "\n";
      }
      file.close();
    }
    void Load_World(Texture &texture){
      std::ifstream file;
      file.open("map.txt");
      float x;
      float y;
      float width;
      float height;
      float sourcex;
      float sourcey;
      bool solid;
      while (file >> x >> y >> width >> height >> sourcex >> sourcey >> solid){
        std::shared_ptr<RigidBody2D>obj = std::make_shared<RigidBody2D>();
        obj->position.x = x;
        obj->position.y = y;
        obj->size.x = width;
        obj->size.y = height;
        obj->source = {sourcex, sourcey, 32, 32};
        obj->is_static = solid;
        obj->texture = texture;
        obj->color = RED;
        objects.push_back(obj);
      }
      file.close();
    }
    void Print_World(){
      for (auto obj : objects){
        std::cout <<
        obj->position.x << " " <<
        obj->position.y << " " <<
        obj->size.x << " " <<
        obj->size.y << " " <<
        obj->is_static << std::endl;
      }
    }
    void Resolve_World_Collision(){
      Vector2 sign = {0,0};
      bool collided;
      Rectangle collision;
      for (auto i : objects){
        // std::cout << sign.x << "," << sign.y << std::endl;
        for (auto j : objects){
          if (i == j){
            break;
          }
          collided = CheckCollisionRecs(i->collider, j->collider);
          collision = GetCollisionRec(i->collider, j->collider);
          if (collided){
            sign.x = i->collider.x + i->collider.width < j->collider.x + j->collider.width ? 1 : -1;
            sign.y = i->collider.y + i->collider.height < j->collider.y + j->collider.height ? 1 : -1;
            if (collision.width < collision.height){
              j->position.x += collision.width * sign.x;
              j->is_grounded = false;
            }
            else if (collision.height < collision.width){
              j->is_grounded = true;
              j->position.y += collision.height * sign.y;
              j->velocity.y = 0;
            }
          }
        }
        i->is_grounded = false;
      }
    }
};
class Editor{
  
};
void Draw_Grid(World world);
void Game();
bool Is_Hovered(Rectangle body, Vector2 mouse){
  bool hovered;
  if (mouse.x > body.x and mouse.x < body.x + body.width and
      mouse.y > body.y and mouse.y < body.y + body.height){
    hovered = true;
  }
  else {
    hovered = false;
  }
  return hovered;
}

int main(){
  int scr_width = 1280;
  int scr_height = 720;
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

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
  world.grid_count = (scr_width/32) + (scr_height / 32);
  world.grid_size = 32;
  
  std::cout << "count: " << world.objects.size() << std::endl;


  Atlas atlas;
  atlas.grid_size = 32;
  atlas.texture = LoadTexture("forestgroundtileset.png");
  world.Load_World(atlas.texture);

  Vector2 source;
  
  while (!WindowShouldClose()){
    //update
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
    // std::cout << mouse.x/32 << "," << mouse.y/32 << std::endl;
    world.Update();
    if (IsKeyDown(KEY_LEFT)){
      camera.target.x -= speed * delta;
    }
    else if (IsKeyDown(KEY_RIGHT)){
      camera.target.x += speed * delta;
    }
    if (IsKeyDown(KEY_UP)){
      camera.target.y -= speed * delta;
    }
    else if (IsKeyDown(KEY_DOWN)){
      camera.target.y += speed * delta;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) and GetMousePosition().x >= scr_width - 350){
      // subtracted 26 for offset
      source = {float(GetMouseX()/32 - 29)*32, float(GetMouseY() / 32)*32};
      std::cout << "source: " << source.x << "," << source.y << std::endl;

    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) and GetMousePosition().x < scr_width - 350 ){
      std::shared_ptr<RigidBody2D>obj = std::make_shared<RigidBody2D>();
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x*32), float(position_y*32) };
      obj->size = {32, 32};
      obj->color = RED;
      obj->source = {source.x, source.y, 32, 32};
      obj->texture = atlas.texture;
      obj->is_static = true;
      world.objects.push_back(obj);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
      std::shared_ptr<RigidBody2D>obj = std::make_shared<RigidBody2D>();
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x*32), float(position_y*32) };
      obj->size = {32, 32};
      obj->color = YELLOW;
      obj->is_static = false;
      world.objects.push_back(obj);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
      int pointx = int(mouse.x);
      int pointy = int(mouse.y);
      std::cout << pointx << " " << pointy << std::endl;
      for (int i = 0; i < world.objects.size(); i++){
        RigidBody2D obj = *world.objects[i];
        if(mouse.x > obj.position.x and mouse.x < obj.position.x + obj .size.x and mouse.y > obj.position.y and mouse.y < obj.position.y + obj.size.y){
        //   std::cout << "position: " << obj.position.x << "," << obj.position.y << std::endl;
          std::cout << i << std::endl;
          world.objects.erase(world.objects.begin()+i);
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

    // world.Draw();
    for (auto i : world.objects){
      i->draw();
    }
    Draw_Grid(world);

    EndMode2D();
    EndTextureMode();
    
    // draw
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawTextureRec(viewport.texture, screen_rect, {0,0}, WHITE);
    DrawRectangle(scr_width - 350, 0, 350, scr_height, DARKGRAY);
    DrawTexture(atlas.texture, scr_width - 350, 0, WHITE );
    EndDrawing();
  }
  CloseWindow();
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
  int scr_height = 600;
  InitWindow(scr_width, scr_height, "Collisions");
  SetTargetFPS(fps);

  World world;
  world.grid_count = 25;
  world.grid_size = 32;
  std::shared_ptr<Player>player = std::make_shared<Player>();
  world.objects.push_back(player);
  Rectangle collision;
  Atlas atlas;
  atlas.texture = LoadTexture("forestgroundtileset.png");
  world.Load_World(atlas.texture);

  
  // std::cout << "objects: " << world.objects.size() << std::endl;
  while (!WindowShouldClose()){
    Vector2 mouse = GetMousePosition();
    // update
    world.Resolve_World_Collision();
    world.Update();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
      std::shared_ptr<RigidBody2D>obj = std::make_shared<RigidBody2D>();
      int position_x = int(mouse.x / 32);
      int position_y = int(mouse.y / 32);
      obj->position = {float(position_x*32), float(position_y*32) };
      obj->size = {32, 32};
      obj->color = RED;
      obj->is_static = true;
      world.objects.push_back(obj);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
      std::shared_ptr<RigidBody2D>block = std::make_shared<RigidBody2D>();
      block->position = {GetMousePosition().x, GetMousePosition().y};
      block->size = {32,32};
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
    ClearBackground(DARKGRAY);
    world.Draw();
    EndDrawing();
  }
  world.Print_World();
  CloseWindow();
}
