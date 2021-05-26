#ifndef _DEBUG_INCLUDE
#define _DEBUG_INCLUDE

#include "KdTree.h"
#include "Octree.h"
#include "TileMapLoader.h"
#include "TriangleMesh.h"
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

  static void print(const float &f) {
    std::cout << "float: " << f << std::endl;
  }

  static void print(const glm::vec3 &v) {
    std::cout << "vec3: " << v.x << " " << v.y << " " << v.z << std::endl;
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

  static void print(const glm::vec4 &v) {
    std::cout << "vec4: " << v.x << " " << v.y << " " << v.z << " " << v.w
              << std::endl;
  }
  static void print(const std::string &s) { std::cerr << s << std::endl; }

  static void print(const KdTree &kdtree) {
    std::cerr << "LEVEL: " << kdtree.getLevel() << std::endl;
    for (unsigned int i = 0; i < kdtree.getQttyChildrens(); ++i) {
      std::cerr << "    node:" << kdtree.getQttyElements() << std::endl;
      Debug::print(kdtree.getChildren(i));
    }
    if (kdtree.isLeaf()) {
      std::cerr << "        leave:" << kdtree.getQttyElements() << std::endl;
    }
  }
};

#endif // ifndef _DEBUG_INCLUDE
