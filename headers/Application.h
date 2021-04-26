#ifndef _APPLICATION_INCLUDE
#define _APPLICATION_INCLUDE

#include "Debug.h"
#include "DebugDisplay.h"
#include "Scene.h"

// Application is a singleton (a class with a single instance) that represents
// our whole app

class Application {
public:
  inline static int UNDEFINED_KEY = -1;
  Application() {}

  static Application &instance() {
    static Application G;
    return G;
  }

  void init(const std::string &filename = "");

  bool update(int deltaTime);
  void render();

  void resize(int width, int height);

  // Input callback methods
  void keyEvent(int key, int specialkey, bool pressed);
  void mouseMove(int x, int y);
  void mousePress(int button);
  void mouseRelease(int button);

private:
  bool bPlay;                // Continue?
  Scene scene;               // Scene to render
  DebugDisplay debugDisplay; // Debug info to render

  bool keys[256], specialKeys[256]; // Store key states so that
                                    // we can have access at any time

  glm::ivec2 lastMousePos; // Last Mouse position
  bool mouseButtons[3];    // State of mouse buttons
};

#endif // _APPLICATION_INCLUDE
