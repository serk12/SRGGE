#include "CalcOcclusionMap.h"

#include "Debug.h"
#include "Define.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

void getXYtile(const std::string &filename, int &x, int &y) {
  std::ifstream tilemap_file(filename);
  for (std::string line; std::getline(tilemap_file, line);) {
    std::istringstream buf(line);
    int sizeX = 0;
    for (int element; buf >> element;) {
      ++sizeX;
    }
    if (x < sizeX)
      x = sizeX;
    ++y;
  }
}

void saveResult(const Result &result, int x, int y) {
  std::ofstream file;
  file.open(CalcOcclusionMap::OUTPUT_FILE, std::ios_base::trunc);
  file << x << " " << y << std::endl;
  int j = 1;
  for (const auto &r : result) {
    for (const auto &el : r) {
      file << el << " ";
    }
    file << std::endl;
    ++j;
  }
  file.close();
}

void CalcOcclusionMap::init(int argc, char **argv,
                            const std::string &filename) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow(argv[0]);
  glutReshapeFunc(resizeCallback);
  glutDisplayFunc(callbackRender);
  glewExperimental = GL_TRUE;
  glewInit();
  glClearColor(1.f, 1.f, 1.f, 1.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  this->Occlusion::init(ALL_SAW);
  loadTileMap(filename);
  result = Result(qttyToDone, std::vector<bool>(mMeshes.size(), false));
  glutMainLoop();
}

void CalcOcclusionMap::resizeCallback(int width, int height) {
  CalcOcclusionMap::instance().resize(width, height);
}

void CalcOcclusionMap::callbackRender() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  CalcOcclusionMap::instance().computeVisibility();
  glutSwapBuffers();
  glFinish();
  glutPostRedisplay();
}

void CalcOcclusionMap::computeVisibility() {
  // set position and angle
  mPlayer.setAngle(glm::vec2(0.0f, angle * FOV));
  int min = x < y ? x : y;
  float posX = i % min;
  float posY = i / min;
  mPlayer.setPos(glm::vec3(posY - (y / 2.0f), PLAYER_Y, posX - (x / 2.0f)));
  // paint and query
  testFrustrumCulling();
  mBasicProgram.use();
  mBasicProgram.setUniformMatrix4f("projection", mPlayer.getProjectionMatrix());
  mBasicProgram.setUniformMatrix4f("view", mPlayer.getViewMatrix());
  mBasicProgram.setUniform1i("bLighting", 1);
  mBasicProgram.setUniform4f("color", 0.9f, 0.9f, 0.95f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  for (auto &mesh : mMeshes) {
    mBasicProgram.setUniformMatrix4f("model", mesh->getModelMatrix());
    mesh->render();
  }
  testOcclusion();
  // collect result
  Debug::info(std::to_string(i) + std::string(".") + std::to_string(angle) +
              std::string("/") + std::to_string(qttyToDone));
  int j = 0;
  for (const auto &mesh : mMeshes) {
    result[i][j] = mesh->isVisible() || result[i][j];
    ++j;
  }
  // set up next
  if (angle * FOV == (360 - FOV)) {
    angle = 0;
    ++i;
  } else {
    ++angle;
  }
  // it's done?
  if (i == qttyToDone) {
    saveResult(result, x, y);
    Debug::info("ENDED OK");
    exit(0);
  }
}

CalcOcclusionMap::CalcOcclusionMap() : Occlusion() {}

void CalcOcclusionMap::calcOcclusionMap(int argc, char **argv) {
  std::string filename = std::string(argv[1]);
  std::string formatFile =
      std::string(&filename[filename.size() - 3], &filename[filename.size()]);
  if (formatFile != "txt") {
    Debug::error("wrong file input calc occlusion");
  } else {
    getXYtile(filename, x, y);
    qttyToDone = x * y;
    if (qttyToDone > 0) {
      init(argc, argv, filename);
    } else {
      Debug::error("negative result... something wrong");
    }
  }
}
