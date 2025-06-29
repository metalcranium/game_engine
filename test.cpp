#include <iostream>
#include <print>
#include <format>

template<typename T>
T max(T x, T y){
  return (x<y) ? y:x;
}
template <typename T>
void swap(T &x, T &y){
  T z = x;
  x = y;
  y = z;
}
template <typename T>
struct Pair{
  T first;
  T second;
};
int main(){
  Pair<int>p = {1,2};
  if (p.first < p.second){
    swap(p.first, p.second);
  }
  std::cout << p.first << "," << p.second << std::endl;
}

