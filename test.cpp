#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sqlite3.h>


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
  sqlite3* db;
  sqlite3_stmt* stmt;
  sqlite3_open("test.db", &db);
  int sql;
  std::string query;

  // query = "CREATE TABLE IF NOT EXISTS Tiles("
  //   "name TEXT,"
  //   "x FLOAT,"
  //   "y FLOAT,"
  //   "width INT,"
  //   "height INT"
  //   ");";
  //  sql = sqlite3_exec(db, query.c_str(), NULL, NULL,NULL);

  //  query = "INSERT INTO Tiles Values("
  //      "'blake',"
  //      "25,"
  //      "50,"
  //      "32,"
  //      "32"
  //    ")";

   // sql = sqlite3_exec(db, query.c_str(), NULL, NULL,NULL);
   query = "SELECT name FROM Tiles";

   sql = sqlite3_exec(db, query.c_str(), callback, NULL,NULL);
   std::cout << sql << std::endl;
  


  
}
