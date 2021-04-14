#include "Museum.h"

#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps

#include <GL/glew.h>
#include <GL/glut.h>

// If a key is pressed this callback is called

void Museum::keyboardDownCallback(unsigned char key, int x, int y) {
  Application::instance().keyEvent(key, -1, true);
}

// If a key is released this callback is called

void Museum::keyboardUpCallback(unsigned char key, int x, int y) {
  Application::instance().keyEvent(key, -1, false);
}

// If a special key is pressed this callback is called

void Museum::specialDownCallback(int key, int x, int y) {
  Application::instance().keyEvent(-1, key, false);
}

// If a special key is released this callback is called

void Museum::specialUpCallback(int key, int x, int y) {
  Application::instance().keyEvent(-1, key, true);
}

// Same for changes in mouse cursor position

void Museum::motionCallback(int x, int y) {
  Application::instance().mouseMove(x, y);
}

// Same for mouse button presses or releases

void Museum::mouseCallback(int button, int state, int x, int y) {
  int buttonId;

  switch (button) {
  case GLUT_LEFT_BUTTON:
    buttonId = 0;
    break;

  case GLUT_RIGHT_BUTTON:
    buttonId = 1;
    break;

  case GLUT_MIDDLE_BUTTON:
    buttonId = 2;
    break;
  }

  if (state == GLUT_DOWN)
    Application::instance().mousePress(buttonId);
  else if (state == GLUT_UP)
    Application::instance().mouseRelease(buttonId);
}

// Resizing the window calls this function

void Museum::resizeCallback(int width, int height) {
  Application::instance().resize(width, height);
}

void Museum::drawCallback() {
  Application::instance().render();
  glutSwapBuffers();
}

void Museum::idleCallback() {
  int currentTime = glutGet(GLUT_ELAPSED_TIME);
  int deltaTime = currentTime - prevTime;

  if (deltaTime > TIME_PER_FRAME) {
    // Every time we enter here is equivalent to a game loop execution
    if (!Application::instance().update(deltaTime))
      exit(0);
    prevTime = currentTime;
    glutPostRedisplay();
  }
}

Museum::Museum(int argc, char **argv) {
  // GLUT initialization
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);

  glutCreateWindow(argv[0]);
  glutReshapeFunc(resizeCallback);
  glutDisplayFunc(drawCallback);
  glutIdleFunc(idleCallback);
  glutKeyboardFunc(keyboardDownCallback);
  glutKeyboardUpFunc(keyboardUpCallback);
  glutSpecialFunc(specialDownCallback);
  glutSpecialUpFunc(specialUpCallback);
  glutMouseFunc(mouseCallback);
  glutMotionFunc(motionCallback);

  // GLEW will take care of OpenGL extension functions
  glewExperimental = GL_TRUE;
  glewInit();

  // Application instance initialization
  if (argc > 1)
    Application::instance().init(std::string(argv[1]));
  else
    Application::instance().init();
  prevTime = glutGet(GLUT_ELAPSED_TIME);
  // GLUT gains control of the application
  glutMainLoop();
}
