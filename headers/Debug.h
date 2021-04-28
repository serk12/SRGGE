#ifndef _DEBUG_INCLUDE
#define _DEBUG_INCLUDE

#include "Octree.h"
#include "TileMapLoader.h"
#include <iostream>

class Debug {
public:
  static void print(const TileRowModels &t) {
    for (auto s : t) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }

  static void print(const TileMapModels &t) {
    for (auto r : t) {
      print(r);
    }
  }

  static void print(const Octree &o) { std::cout << o.getQtty() << std::endl; }
};

#endif // ifndef _DEBUG_INCLUDE
