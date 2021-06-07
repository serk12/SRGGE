#ifndef _DEBUGDISPLAY_INCLUDE
#define _DEBUGDISPLAY_INCLUDE

#include "TextRender.h"

class DebugDisplay {
public:
  DebugDisplay();

  void init();
  bool update(int dt);
  bool render();

  void setQttyTriangles(int triangles);

private:
  int deltaTime, qttyTriangles, qtty;
  TextRender textRender;
};

#endif // _DEBUGDISPLAY_INCLUDE
