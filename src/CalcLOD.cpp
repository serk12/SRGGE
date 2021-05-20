#include "CalcLOD.h"

#include "Debug.h"
#include "PLYReader.h"
#include "TriangleMesh.h"

#include <tinyply.h>
#include <vector>
using namespace tinyply;

void modelToPly(const std::string &filename, const TriangleMesh &mesh) {
  std::filebuf fb;
  fb.open(filename + "-binary.ply", std::ios::out | std::ios::binary);
  std::ostream outstream_binary(&fb);
  if (outstream_binary.fail())
    throw std::runtime_error("failed to open " + filename);

  PlyFile mf;
  auto aux_v = mesh.exportVertices();
  mf.add_properties_to_element(
      "vertex", {"x", "y", "z"}, Type::FLOAT32, aux_v.size(),
      reinterpret_cast<uint8_t *>(aux_v.data()), Type::INVALID, 0);

  auto aux = mesh.exportTriangles();
  mf.add_properties_to_element(
      "face", {"vertex_indices"}, Type::UINT32, aux.size(),
      reinterpret_cast<uint8_t *>(aux.data()), Type::UINT8, 3);
  mf.get_comments().push_back("generated by tinyply 2.3");

  mf.write(outstream_binary, true);
}

void cubize(const Octree &octree, TriangleMesh &mesh, int level,
            int maxLevel = -1, bool showCubes = false) {
  if (showCubes) {
    glm::vec3 pos = octree.getPoss();
    glm::vec3 size = octree.getSize();
    mesh.buildCube(pos, size);
  }
  if (octree.getQttyElements() > 0) {
    if (!showCubes) {
      glm::vec3 pos = octree.getPoss();
      glm::vec3 size = octree.getSize();
      mesh.buildCube(pos, size);
    }
  } else if (maxLevel == -1 || level < maxLevel) {
    for (unsigned int i = 0; i < Octree::VECT_SIZE; ++i) {
      const Octree c = octree.getChildren(i);
      if (c.getQtty() >= 1) {
        cubize(c, mesh, level + 1, maxLevel, showCubes);
      }
    }
  }
}

void simplify(const Octree &octree, TriangleMesh &mesh, int actualLevel,
              int level) {
  if (octree.getQttyChildrens() > 0 && actualLevel < level) {
    for (unsigned int i = 0; i < Octree::VECT_SIZE; ++i) {
      simplify(octree.getChildren(i), mesh, actualLevel + 1, level);
    }
  } else if (octree.getQttyElements() > 0) {
    int size = octree.getQttyElements();
    glm::vec3 mean(0.0f);
    for (unsigned int i = 0; i < size; ++i) {
      mean += octree.getElementVec(i);
    }
    mean /= size;
    mesh.buildCube(mean, octree.getSize());
  }
}

CalcLOD::CalcLOD(int argc, char **argv) {
  TriangleMesh *mesh = new TriangleMesh();
  bool bSuccess = PLYReader::readMesh(std::string(argv[1]), *mesh);
  if (not bSuccess) {
    std::cerr << "error loading model" << std::endl;
    return;
  }

  Octree octree(mesh->getVertices());

  TriangleMesh *new_mesh = new TriangleMesh();
  int level = octree.getMaxLevel() - LOD_LEVEL;
  if (LOD_LEVEL == 0) {
    cubize(octree, *new_mesh, 0, -1, false);
  } else if (level > 0) {
    octree.cut(level);
    simplify(octree, *new_mesh, 0, level);
  } else {
    cerr << "Error LOD > octree max level" << std::endl;
  }
  modelToPly("testing", *new_mesh);
}
