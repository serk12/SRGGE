#ifndef _CALCLOD_HEADER
#define _CALCLOD_HEADER

#include <string>

class CalcLOD {
public:
  CalcLOD(int argc, char **argv);
  inline static int LOD_LEVEL = 0;
  inline static bool USE_QEM = true;
  inline static std::string OUTPUT_FILE = "./models/testing-binary.ply";
};

#endif // _CALCLOD_HEADER
