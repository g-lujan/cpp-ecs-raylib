#ifndef _RESOURCES_H__
#define _RESOURCES_H__

#include "graphics.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace Resources {
  class Manager {
  public:
    Manager();
    Graphics::Texture &texture(const std::string tex_name) { return _textures[tex_name]; }
    Graphics::Animation_Settings &animation(const std::string anim_name, const KeyboardKey key) { return _animations[anim_name][key]; }
    std::string tilemap_path(const std::string map_name) { return _tilemaps_paths[map_name]; }

    // May implement ways to clear unused stuff from the Manager in the future
    // For now, the manager will be unique and will serve all views

  private:
    std::unordered_map<std::string, Graphics::Texture> _textures;
    std::unordered_map<std::string, std::unordered_map<KeyboardKey, Graphics::Animation_Settings>> _animations;
    std::unordered_map<std::string, std::string> _tilemaps_paths;
  };

  Manager &get_resource_manager();

  namespace Paths {
    inline constexpr auto BASE_PATH = "resources/images/";
    inline constexpr auto PLAYER_RED = "resources/images/red.png";
    inline constexpr auto HOMETOWN_IMAGE = "resources/images/hometown.png";
    inline constexpr auto HOMETOWN_JSON = "resources/images/ville.json";
  } // namespace Paths
} // namespace Resources

#endif
