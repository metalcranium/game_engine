#include <iostream>
#include <fstream>
// #include <stdio.h>
#include <string>
#include <vector>
#include <sqlite3.h>


struct Tile{
  std::string name;
  float x;
  float y;
  float width;
  float height;
};
struct Object{
  std::string name;
  float x;
  float y;
  float width;
  float height;
};
struct World{
  std::vector<Tile>tiles;
  std::vector<Object>objects;
};
World world;

void save(World world){
  std::ofstream file;
  file.open("test.txt");
  for (auto i : world.tiles){
    file << i.name << " ";
    file << i.x << " ";
    file << i.y << " ";
    file << i.width << " ";
    file << i.height << " ";
  }
  file << "\n";
  for (auto i : world.objects){
    file << i.name << " ";
    file << i.x << " ";
    file << i.y << " ";
    file << i.width << " ";
    file << i.height << " ";
  }
  file.close();  
}
  
void load(World &world){
  std::ifstream file;
  file.open("test.txt");
  std::string item;
  std::string text;
  std::string name;
  float x;
  float y;
  float width;
  float height;
  std::vector<std::string>items;
  while (file>> name >> x >> y >> width >> height){
    Tile tile;
    tile.name = name;
    tile.x = x;
    tile.y = y;
    tile.width = width;
    tile.height = height;
    world.tiles.push_back(tile);
  }
  file.close();
}
void print_world(World world){
  for (auto i : world.tiles){
    std::cout << i.name << std::endl;
    std::cout << i.x << std::endl;
    std::cout << i.y << std::endl;
    std::cout << i.width << std::endl;
    std::cout << i.height << std::endl;
  }
  for (auto i : world.objects){
    std::cout << i.name << std::endl;
    std::cout << i.x << std::endl;
    std::cout << i.y << std::endl;
    std::cout << i.width << std::endl;
    std::cout << i.height << std::endl;
  }
}
int callback(void* NotUsed, int argc, char** argv, char** ColName){
  for (int i = 0; i < argc; i++){
    std::cout << ColName[i] << ": " << argv[i] << std::endl;
  }
  return 0;
}
int loadin(void* NotUsed, int argc, char** argv, char** ColName){
  
  return 0;
}

int main(){
  // Tile tile;
  // tile.name = "grass";
  // tile.x = 25;
  // tile.y = 30;
  // tile.width = 32;
  // tile.height = 32;
  // world.tiles.push_back(tile);
  // Object obj;
  // obj.name = "player";
  // obj.x = 77;
  // obj.y = 42;
  // obj.width = 32;
  // obj.height = 32;
  // world.objects.push_back(obj);
  // sqlite3* db;
  // sqlite3_stmt* stmt;
  // sqlite3_open("test.db", &db);
  // int sql;

  // std::string query = "CREATE TABLE IF NOT EXISTS Tiles("
  //   "name TEXT,"
  //   "x FLOAT,"
  //   "y FLOAT,"
  //   "width INT,"
  //   "height INT"
  //   ");";
  //  sql = sqlite3_exec(db, query.c_str(), NULL, NULL,NULL);

   // query = "INSERT INTO Tiles Values("
   //     "'blake',"
   //     "25,"
   //     "50,"
   //     "32,"
   //     "32"
   //   ")";

   // sql = sqlite3_exec(db, query.c_str(), NULL, NULL,NULL);
   // query = "SELECT name FROM Tiles";

   // sql = sqlite3_exec(db, query.c_str(), callback, NULL,NULL);
   // std::cout << sql << std::endl;
  


  // save(world);
  load(world);
  print_world(world);
  
}
