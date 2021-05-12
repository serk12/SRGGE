#include "CalcLOD.h"
#include "Museum.h"
#include <cstring>

enum ApplicationType { MUSEUM, LOD };
static const char *CULLING_FLAG = "--CULLING";
static const char *LOD_FLAG = "--calc-LOD";

int main(int argc, char **argv) {
  ApplicationType application = ApplicationType::MUSEUM;
  for (unsigned int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], LOD_FLAG) == 0) {
      application = ApplicationType::LOD;
      if (argc >= i + 1) {
        CalcLOD::LOD_LEVEL = std::stoi(argv[i + 1]);
      } else {
        std::cout << "ERROR LOD flag" << std::endl;
        break;
      }

    } else if (strcmp(argv[i], CULLING_FLAG) == 0) {
      if (argc >= i + 1 && argv[i + 1][0] >= '0' && argv[i + 1][0] <= '3') {
        Application::CULLING_POLICY = CullingMethod(std::stoi(argv[i + 1]));
      } else {
        std::cout << "ERROR culling flag" << std::endl;
        break;
      }
    }
  }
  switch (application) {
  case ApplicationType::MUSEUM:
    new Museum(argc, argv);
    break;
  case ApplicationType::LOD:
    new CalcLOD(argc, argv);
    break;
  default:
    break;
  }

  return 0;
}
