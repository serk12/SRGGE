#include "CalcLOD.h"
#include "Museum.h"
#include <cstring>

enum ApplicationType { MUSEUM, LOD };
static const char *CULLING_FLAG = "--CULLING";
static const char *LOD_FLAG = "--calc-LOD";

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
  std::string lastParameter = argv[argc - 1];
  std::string file_format =
      std::string(&lastParameter[lastParameter.size() - 3],
                  &lastParameter[lastParameter.size()]);

  if (file_format == "ply" || file_format == "txt") {
    CalcLOD::OUTPUT_FILE = lastParameter;
  }
  return application;
}

int main(int argc, char **argv) {
  switch (parseParameters(argc, argv)) {
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
