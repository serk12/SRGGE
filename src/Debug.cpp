#include "Debug.h"

void Debug::error(const std::string &s) {
  std::cerr << "ERROR: " << s << std::endl;
}

void Debug::info(const std::string &s) { std::cout << s << std::endl; }

void Debug::print(const TileRowModels &t) {
  for (auto s : t) {
    std::cout << s << " ";
  }
  std::cout << std::endl;
}

void Debug::print(const TileMapModels &t) {
  for (auto r : t) {
    print(r);
  }
}

void Debug::print(const glm::vec3 &v) {
  std::cout << "vec3: " << v.x << " " << v.y << " " << v.z << std::endl;
}

void Debug::print(const std::vector<glm::vec3> &v) {
  for (const auto &p : v) {
    Debug::print(p);
  }
}

void Debug::print(const Octree &o) {
  std::cout << "QTTY: " << o.getQtty() << " ELEMENTS: " << o.getQttyElements()
            << " SIZE: ";
  print(o.getSize());
  std::cout << " POSS ";
  print(o.getPoss());
  std::cout << std::endl;

  for (int i = 0; i < Octree::VECT_SIZE; ++i) {
    const Octree c = o.getChildren(i);
    std::cout << "LEVEL: " << c.getLevel() << " CHILD: " << i
              << " QTTY: " << c.getQtty() << " ELEMENTS:" << c.getQttyElements()
              << " SIZE: ";
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

void Debug::print(const glm::vec4 &v) {
  std::cout << "vec4: " << v.x << " " << v.y << " " << v.z << " " << v.w
            << std::endl;
}
void Debug::print(const std::string &s) { std::cerr << s << std::endl; }

void Debug::print(const float &f) { std::cout << "float: " << f << std::endl; }

void Debug::print(const KdTree &kdtree) {
  std::cerr << "LEVEL: " << kdtree.getLevel() << std::endl;
  Debug::print(kdtree.getAxis());

  if (kdtree.isLeaf()) {
    std::cerr << "    leave:" << kdtree.getQttyElements() << std::endl;
  } else {
    std::cerr << "    node:" << kdtree.getQttyElements() << std::endl;
  }
  for (unsigned int i = 0; i < kdtree.getQttyElements(); ++i) {
    std::cerr << " element: " << i << " " << kdtree.getElement(i)->getName()
              << std::endl;
  }
  for (unsigned int i = 0; i < kdtree.getQttyChildrens(); ++i) {
    auto k = *kdtree.getChildren(i);
    Debug::print(k);
  }
}
