#ifndef _MUSEUM_INCLUDE
#define _MUSEUM_INCLUDE

#include "Application.h"

// class defining glew callbacks linking
class Museum {

public:
  Museum(int argc, char **argv);

private:
  // This object represents our whole app
  inline static Application app;
  inline static int prevTime;

  static void keyboardDownCallback(unsigned char key, int x, int y);
  static void keyboardUpCallback(unsigned char key, int x, int y);
  static void specialDownCallback(int key, int x, int y);
  static void specialUpCallback(int key, int x, int y);
  static void motionCallback(int x, int y);
  static void mouseCallback(int button, int state, int x, int y);
  static void resizeCallback(int width, int height);
  static void drawCallback();
  static void idleCallback();
};

#endif // _MUSEUM_INCLUDE
