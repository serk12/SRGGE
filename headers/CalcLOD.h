#ifndef _CALCLOD_HEADER
#define _CALCLOD_HEADER

#include <string>

struct CalcLOD {
  CalcLOD(int argc, char **argv);
  static bool USE_QEM;
  inline static int LOD_LEVEL = 0;
  inline static std::string OUTPUT_FILE = "./models/testing-binary.ply";
};

#endif // _CALCLOD_HEADER
