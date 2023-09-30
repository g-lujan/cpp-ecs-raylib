#ifndef _RESOURCES_H__
#define _RESOURCES_H__

#include "../graphics/graphics.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>

namespace Resources {
  namespace Paths {
    inline constexpr auto BASE_PATH = "resources/images/";
    inline constexpr auto PLAYER_RED = "resources/images/red.png";
    inline constexpr auto HOMETOWN_IMAGE = "resources/images/hometown.png";
    inline constexpr auto HOMETOWN_JSON = "resources/images/ville.json";
  } // namespace Paths

  class Manager {
  public:
    Manager();
    Graphics::Texture &texture(const std::string tex_name);
    Graphics::Frame &animation(const std::string anim_name, const std::string action);
    std::string tilemap_path(const std::string map_name);

    // May implement ways to clear unused stuff from the Manager in the future
    // For now, the manager will be unique and will serve all views

  private:
    std::unordered_map<std::string, Graphics::Texture> _textures;
    std::unordered_map<std::string, std::unordered_map<std::string, Graphics::Frame>> _animations;
    std::unordered_map<std::string, std::string> _tilemaps_paths;
  };

  Manager &get_resource_manager();
} // namespace Resources

#endif
