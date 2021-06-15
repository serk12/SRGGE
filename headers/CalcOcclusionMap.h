#ifndef _CALC_OCCLUSION_MAP_INCLUDE
#define _CALC_OCCLUSION_MAP_INCLUDE

#include "Occlusion.h"
#include <string>

using Result = std::vector<std::vector<bool>>;

struct CalcOcclusionMap : public Occlusion {
  CalcOcclusionMap();

  static CalcOcclusionMap &instance() {
    static CalcOcclusionMap G;
    return G;
  }

  void calcOcclusionMap(int argc, char **argv);
  inline static std::string OUTPUT_FILE = "./resources/occlusionmap.txt";

private:
  void init(int argc, char **argv, const std::string &filename);
  void computeVisibility();
  static void callbackRender();
  static void resizeCallback(int width, int height);
  Result result;
  int i = 0, angle = 0, x = 0, y = 0;
  unsigned int qttyToDone;
};

#endif
