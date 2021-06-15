#ifndef _MANAGER_LOD_INCLUDE
#define _MANAGER_LOD_INCLUDE

#include <list>

class Player;
class Mesh;
class ManagerLOD {
public:
  ManagerLOD();
  int getQttyTriangles() const;

protected:
  void update(int deltaTime, std::list<Mesh *> &meshes, const Player &player);

protected:
  unsigned int mQttyTriangles, mQttyMean;
  static const unsigned int QTTY_MEAN;

private:
  unsigned int mQttyTrianglesTopFPS, mHisteresyCount;
  static const unsigned int HISTERESY_ELAPSE_TIME;
  static const float HISTERESY_MODELS_COUNT;
};

#endif
