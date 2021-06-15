#ifndef _OCCLUSION_INCLUDE
#define _OCCLUSION_INCLUDE

#include "BasicScene.h"
#include "KdTree.h"
#include <string>

enum CullingMethod {
  NONE = 0,
  VIEW = 1,
  OCCLUSION = 2,
  ALL = 3,
  OCCLUSION_SAW = 4,
  ALL_SAW = 5,
  CULLING_MAP = 6
};

struct Query {
  KdTree *tree;
  int qttyVisiblePixels;
  GLuint queryID;
  bool done;

  Query(KdTree *tree);
  bool getQuery();
  void waitQuery();
};

class Occlusion : public BasicScene {
public:
  Occlusion(CullingMethod cm = NONE);

  void init(CullingMethod cm);
  void render() const;
  void update(int deltaTime);
  static void setOcclusionMap(const std::string &map);

protected:
  bool viewCulling(const Mesh &mesh);
  void occlusionCulling();
  void occlusionCullingSaW();
  void keyEvent(int key, int specialkey, bool pressed);
  void testFrustrumCulling();
  void testOcclusion();

private:
  int mBKDTree;
  unsigned int mFrame;
  CullingMethod mCullingPolicy;
  KdTree *mKDTree;
  static const int VISIBLE_PIXELS_THRESHOLD;
  inline static std::string OCCLUSION_MAP = "";
  inline static std::vector<std::vector<bool>> msOcclusionMap;
  inline static int msX, msY;
};

#endif
