#include "Museum.h"
#include <cstring>

enum ApplicationType { MUSEUM };

int main(int argc, char **argv) {
  ApplicationType application = ApplicationType::MUSEUM;
  switch (application) {
  case ApplicationType::MUSEUM:
    new Museum(argc, argv);
    break;
  default:
    break;
  }

  return 0;
}
