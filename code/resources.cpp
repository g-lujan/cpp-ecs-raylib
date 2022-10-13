#include "resources.hpp"

namespace Resources {
  // both need to be init after OpenGL context
  std::unordered_map<std::string, Texture> textures;
  std::unordered_map<Map, std::vector<Tile>> tiles;
  std::vector<std::unordered_map<KeyboardKey, Animation>> animations;
} // namespace Resources
