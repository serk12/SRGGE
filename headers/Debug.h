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

  static void print(const Octree &o, int index = 0) {
    std::cout << "QTTY: " << o.getQtty() << " ELEMENTS: " << o.getQttyElements()
              << std::endl;

    for (int i = 0; i < Octree::VECT_SIZE; ++i) {
      int id = index * Octree::VECT_SIZE + i;
      const Octree c = o.getChildren(i);
      std::cout << "CHILD: " << i << " QTTY: " << c.getQtty()
                << " ELEMENTS:" << c.getQttyElements() << std::endl;
      if (c.getQtty() > 1) {
        Debug::print(c, id);
      }
    }
  }
};

#endif // ifndef _DEBUG_INCLUDE
