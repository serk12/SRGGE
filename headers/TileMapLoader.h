#ifndef _TILE_MAP_LOADER_INCLUDE
#define _TILE_MAP_LOADER_INCLUDE

#include <map>
#include <vector>

using TileRowModels = std::vector<std::string>;
using TileMapModels = std::vector<TileRowModels>;

class TileMapLoader {
public:
  inline static const std::string EMPTY = "empty";
  inline static const std::string WALL = "models/wall.ply";
  inline static const std::string GROUND = "models/ground.ply";

  static TileMapLoader &instance() {
    static TileMapLoader G;
    return G;
  }
  TileMapModels load(const std::string &filename);

private:
  inline static const std::map<int, std::string> ID_TO_FILE = {
      {0, EMPTY},
      {1, WALL},
      {2, GROUND},
      {3, "models/bunny.ply"},
      {4, "models/frog.ply"},
      {5, "models/horse.ply"}};
};

#endif // ifndef _TILE_MAP_LOADER_INCLUDE
