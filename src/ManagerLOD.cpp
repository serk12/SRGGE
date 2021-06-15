#include "ManagerLOD.h"

#include "Define.h"
#include "Mesh.h"
#include "Player.h"
#include <queue>

const unsigned int ManagerLOD::HISTERESY_ELAPSE_TIME = 250; // 0.25s = 250
const float ManagerLOD::HISTERESY_MODELS_COUNT = 0.10;
const unsigned int ManagerLOD::QTTY_MEAN = 100;

struct Benefit {
  float benefit;
  int triangles;
  Mesh *mesh;

  Benefit(float b, int t, Mesh *m) {
    benefit = b;
    triangles = t;
    mesh = m;
  }
};

struct Compare {
  bool operator()(Benefit a, Benefit b) {
    return a.benefit < b.benefit || a.triangles < b.triangles;
  }
};

using BenefitQueue =
    std::priority_queue<Benefit, std::vector<Benefit>, Compare>;

ManagerLOD::ManagerLOD()
    : mQttyTriangles(0), mHisteresyCount(0), mQttyMean(1),
      mQttyTrianglesTopFPS(0) {}
int ManagerLOD::getQttyTriangles() const { return mQttyTriangles / mQttyMean; }

void ManagerLOD::update(int deltaTime, std::list<Mesh *> &meshes,
                        const Player &player) {
  int meanQttyTriangles = mQttyTriangles / mQttyMean;
  mHisteresyCount += deltaTime;
  if (mHisteresyCount >= HISTERESY_ELAPSE_TIME) {
    mHisteresyCount -= HISTERESY_ELAPSE_TIME;
    int maxCostTriangles = meanQttyTriangles;
    // rememver the max qttyTriangles with ~ FPS fps
    if (1.0f / deltaTime * 1000.0f >= 0.9f * (1.0f / FPS) &&
        meanQttyTriangles > mQttyTrianglesTopFPS) {
      mQttyTrianglesTopFPS = meanQttyTriangles;
    } else if (mQttyTrianglesTopFPS > 0) {
      maxCostTriangles = mQttyTrianglesTopFPS;
    }
    float maxCost = ((maxCostTriangles / deltaTime * 1000.0f) * FPS);
    bool decrease = meanQttyTriangles > maxCost;
    BenefitQueue benefitQueue;
    for (auto &m : meshes) {
      if (m->isVisible()) {
        Benefit benefit(m->getMaxBenefit(player.getPos(), decrease),
                        m->getTriangleSize(), m);
        benefitQueue.push(benefit);
      } else {
        m->increaseLOD();
      }
    }
    float totalCost = meanQttyTriangles;
    int qttyChanges = 0;
    while (!benefitQueue.empty()) {
      auto benefit = benefitQueue.top();
      benefitQueue.pop();
      if (decrease) {
        if (totalCost <= maxCost) {
          break;
        } else {
          benefit.mesh->decreaseLOD();
          totalCost += benefit.triangles - benefit.mesh->getTriangleSize();
        }
      } else {
        if (totalCost > maxCost) {
          break;
        } else {
          totalCost += benefit.triangles - benefit.mesh->getTriangleSize();
          benefit.mesh->increaseLOD();
        }
      }
      ++qttyChanges;
      if (qttyChanges >= meshes.size() * HISTERESY_MODELS_COUNT) {
        break;
      }
    }
  }
}
