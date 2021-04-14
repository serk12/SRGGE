#include "CalcLOD.h"
#include "Museum.h"
#include <cstring>

enum ApplicationType { MUSEUM, LOD };
static const char *LOD_FLAG = "--calc-LOD";

int main(int argc, char **argv) {
  ApplicationType application = ApplicationType::MUSEUM;
  for (unsigned int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], LOD_FLAG) == 0) {
      application = ApplicationType::LOD;
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
