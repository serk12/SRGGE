#include <cstring>

#include "CalcLOD.h"
#include "CalcOcclusionMap.h"
#include "Debug.h"
#include "Museum.h"

enum ApplicationType { MUSEUM, LOD, COLLISION_MAP };
static const char *CULLING_FLAG = "--CULLING";
static const char *LOD_FLAG = "--calc-LOD";
static const char *MAP_FLAG = "--calc-COLL-MAP";

ApplicationType parseParameters(int argc, char **argv) {
  ApplicationType application = ApplicationType::MUSEUM;
  for (unsigned int i = 1; i < argc; ++i) {
  }
  for (unsigned int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], LOD_FLAG) == 0) {
      application = ApplicationType::LOD;
      if (argc >= i + 1) {
        CalcLOD::LOD_LEVEL = std::stoi(argv[i + 1]);
      } else {
        Debug::error("ERROR LOD flag");
        break;
      }
    } else if (strcmp(argv[i], CULLING_FLAG) == 0) {
      if (argc >= i + 1 && argv[i + 1][0] >= '0' && argv[i + 1][0] <= '6') {
        Application::CULLING_POLICY = CullingMethod(std::stoi(argv[i + 1]));
        if (argv[i + 1][0] == '6') {
          Occlusion::setOcclusionMap(std::string(argv[i + 2]));
        }
      } else {
        Debug::error("ERROR culling flag");
        break;
      }
    } else if (strcmp(argv[i], MAP_FLAG) == 0) {
      application = ApplicationType::COLLISION_MAP;
    }
  }
  std::string lastParameter = argv[argc - 1];
  std::string file_format =
      std::string(&lastParameter[lastParameter.size() - 3],
                  &lastParameter[lastParameter.size()]);

  if (file_format == "ply") {
    CalcLOD::OUTPUT_FILE = lastParameter;
  }
  if (file_format == "txt") {
    CalcOcclusionMap::OUTPUT_FILE = lastParameter;
  }
  return application;
}

int main(int argc, char **argv) {
  switch (parseParameters(argc, argv)) {
  case ApplicationType::MUSEUM:
    Museum(argc, argv);
    break;
  case ApplicationType::LOD:
    CalcLOD(argc, argv);
    break;
  case ApplicationType::COLLISION_MAP:
    CalcOcclusionMap::instance().calcOcclusionMap(argc, argv);
    break;
  default:
    break;
  }
  return 0;
}
