#ifndef PLYREADER_H
#define PLYREADER_H

#include "TriangleMesh.h"
#include <fstream>

using namespace std;

class PLYReader {
public:
  static bool readMesh(const string &filename, TriangleMesh &mesh);

private:
  static bool loadHeader(ifstream &fin, int &nVertices, int &nFaces);
  static void loadVertices(ifstream &fin, int nVertices,
                           vector<float> &plyVertices);
  static void loadFaces(ifstream &fin, int nFaces, vector<int> &plyTriangles);
  static void rescaleModel(vector<float> &plyVertices);
  static void addModelToMesh(const vector<float> &plyVertices,
                             const vector<int> &plyTriangles,
                             TriangleMesh &mesh);
};

#endif // PLYREADER_H
