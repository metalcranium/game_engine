#include "utility.h"
#include <raylib.h>

float Get_Angle(Vector2 to, Vector2 from){
  return atan2(to.y - from.y, to.x - from.x); 
}
float Get_Rotation(float angle){
  return angle * (180/M_PI) + 180; 
}
float Direction_To(Vector2 to, Vector2 from){
  float angle = Get_Angle(to,from);
  return Get_Rotation(angle);
}
