#include "CalcLOD.h"

#include "Debug.h"
#include "PLYReader.h"
#include "TriangleMesh.h"

CalcLOD::CalcLOD(int argc, char **argv) {
  TriangleMesh *mesh = new TriangleMesh();
  bool bSuccess = PLYReader::readMesh(std::string(argv[1]), *mesh);
  std::cout << (mesh->getVertices()).size() << std::endl;
  Octree octree(mesh->getVertices());
  Debug::print(octree);
  std::cout << (mesh->getVertices()).size() << std::endl;
}
