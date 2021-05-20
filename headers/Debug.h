#ifndef _DEBUG_INCLUDE
#define _DEBUG_INCLUDE

#include "Octree.h"
#include "TileMapLoader.h"
#include <iostream>

class Debug {
private:
  inline static int qtty = 0;

public:
  static void cumulative(int n) { qtty += n; }
  static void printQtty() { print(qtty); }
  static void print(const TileRowModels &t) {
    for (auto s : t) {
      std::cerr << s << " ";
    }
    std::cerr << std::endl;
  }

  static void print(const TileMapModels &t) {
    for (auto r : t) {
      print(r);
    }
  }

  static void print(const int i) { std::cerr << i << std::endl; }
  static void print(const std::string s) { std::cerr << s << std::endl; }

  static void print(const std::vector<Vertex> &vec) {
    for (const auto &v : vec) {
      std::cerr << v.index << " ";
    }
    std::cerr << std::endl;
  }

  static void print(const glm::vec3 &v) {
    std::cerr << v.x << " " << v.y << " " << v.z;
  }

  static void print(const Octree &o) {
    std::cerr << "QTTY: " << o.getQtty() << " ELEMENTS: " << o.getQttyElements()
              << " SIZE: ";
    print(o.getSize());
    std::cerr << " POSS ";
    print(o.getPoss());
    std::cerr << std::endl;

    for (int i = 0; i < Octree::VECT_SIZE; ++i) {
      const Octree c = o.getChildren(i);
      std::cerr << "LEVEL: " << c.getLevel() << " CHILD: " << i
                << " QTTY: " << c.getQtty()
                << " ELEMENTS:" << c.getQttyElements() << " SIZE: ";
      print(c.getSize());
      std::cerr << " POSS: ";
      print(c.getPoss());
      std::cerr << std::endl;
      if (c.getQtty() > 1) {
        Debug::print(c);
      }
    }
    std::cerr << "END CHILD" << std::endl;
    std::cerr << "Max level: " << o.getMaxLevel() << std::endl;
  }
};

#endif // ifndef _DEBUG_INCLUDE
