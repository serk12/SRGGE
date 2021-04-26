#include "Debug.h"
#include "TileMapLoader.h"
#include <fstream>
#include <sstream>

TileMapModels TileMapLoader::load(const std::string &filename) {
  std::ifstream tilemap_file(filename);
  TileMapModels tilemap;
  for (std::string line; std::getline(tilemap_file, line);) {
    std::istringstream buf(line);
    TileRowModels row;
    for (int element; buf >> element;) {
      row.push_back(TileMapLoader::ID_TO_FILE.at(element));
    }
    tilemap.push_back(row);
  }
  return tilemap;
}
