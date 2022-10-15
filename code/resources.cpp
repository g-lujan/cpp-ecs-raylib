#include "resources.hpp"

namespace Resources {
  std::unordered_map<std::string, Texture> textures;
  std::unordered_map<Map, std::vector<Tile>> tiles;
  std::unordered_map<Map, std::string> map_json_path = {{Map::HOMETOWN, "./resources/images/ville.json"}};
  std::vector<std::unordered_map<KeyboardKey, Animation>> animations;
} // namespace Resources
