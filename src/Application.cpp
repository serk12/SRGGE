#include <GL/glew.h>
#include <GL/glut.h>
#include "Application.h"

void Application::init(const std::string& filename)
{
    bPlay = true;
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    scene.init(filename);
    debugDisplay.init();
    for (unsigned int i = 0; i < 256; i++)
    {
        keys[i]        = false;
        specialKeys[i] = false;
    }
    mouseButtons[0] = false;
    mouseButtons[1] = false;
    lastMousePos    = glm::ivec2(-1, -1);
}

bool Application::update(int deltaTime)
{
    scene.update(deltaTime);
    debugDisplay.update(deltaTime);
    return bPlay;
}

void Application::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.render();
    debugDisplay.render();
}

void Application::resize(int width, int height)
{
    glViewport(0, 0, width, height);
    scene.resize(width, height);
}

void Application::keyEvent(int key, int specialkey, bool pressed)
{
    // 27 == escape code
    if (key == 27) bPlay = false;
    scene.keyEvent(key, specialkey, pressed);

    if (specialkey != UNDEFINED_KEY) {
        specialKeys[specialkey] = pressed;
    }
    if (key != UNDEFINED_KEY) {
        keys[key] = pressed;
    }
}

void Application::mouseMove(int x, int y)
{
    scene.mouseMove(x, y, lastMousePos, mouseButtons);
    lastMousePos = glm::ivec2(x, y);
}

void Application::mousePress(int button)
{
    mouseButtons[button] = true;
}

void Application::mouseRelease(int button)
{
    mouseButtons[button] = false;
    if (!mouseButtons[0] && !mouseButtons[1]) lastMousePos = glm::ivec2(-1, -1);
}
