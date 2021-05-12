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

  static void print(const glm::vec3 &v) {
    std::cout << v.x << " " << v.y << " " << v.z;
  }

  static void print(const Octree &o) {
    std::cout << "QTTY: " << o.getQtty() << " ELEMENTS: " << o.getQttyElements()
              << " SIZE: ";
    print(o.getSize());
    std::cout << " POSS ";
    print(o.getPoss());
    std::cout << std::endl;

    for (int i = 0; i < Octree::VECT_SIZE; ++i) {
      const Octree c = o.getChildren(i);
      std::cout << "LEVEL: " << c.getLevel() << " CHILD: " << i
                << " QTTY: " << c.getQtty()
                << " ELEMENTS:" << c.getQttyElements() << " SIZE: ";
      print(c.getSize());
      std::cout << " POSS: ";
      print(c.getPoss());
      std::cout << std::endl;
      if (c.getQtty() > 1) {
        Debug::print(c);
      }
    }
    std::cout << "END CHILD" << std::endl;
    std::cout << "Max level: " << o.getMaxLevel() << std::endl;
  }
};

#endif // ifndef _DEBUG_INCLUDE
