#include "resources.hpp"
#include "../graphics/graphics.hpp"

#include <utility>

namespace Resources {
  Manager::Manager()
  {
    _textures.insert(std::make_pair("empty", Graphics::Texture()));
    _textures.emplace("player", Paths::PLAYER_SLIME);
    _textures.emplace("hometown", Paths::HOMETOWN_IMAGE);
    _animations.emplace(std::make_pair(
        "player",
        std::unordered_map<std::string, Graphics::Frame>(
            {std::make_pair("idle_right", Graphics::Frame("idle_right", Vector2{0, 32 * 0}, 3)),
             std::make_pair("idle_left", Graphics::Frame("idle_left", Vector2{0, 32 * 0}, 3)),
             std::make_pair("move_right", Graphics::Frame("move_right", Vector2{0, 32 * 1}, 15)),
             std::make_pair("move_left", Graphics::Frame("move_left", Vector2{0, 32 * 1}, 15)),
             std::make_pair("move_down", Graphics::Frame("move_down", Vector2{0, 32 * 2}, 15)), 
             std::make_pair("move_up", Graphics::Frame("move_up", Vector2{0, 32 * 3}, 15)),
             std::make_pair("idle_down", Graphics::Frame("idle_down", Vector2{0, 32 * 4}, 3)),
             std::make_pair("idle_up", Graphics::Frame("idle_up", Vector2{0, 32 * 5}, 3))
            }))
    );
    _tilemaps_paths.emplace("hometown", Paths::HOMETOWN_JSON);
  }

  Graphics::Texture &Manager::texture(const std::string &tex_name) { return _textures[tex_name]; }

  Graphics::Frame &Manager::animation(const std::string &anim_name, const std::string &action) { return _animations[anim_name][action]; }

  std::string Manager::tilemap_path(const std::string &map_name) { return _tilemaps_paths[map_name]; }

  Manager &get_resource_manager()
  {
    static Manager manager;
    return manager;
  }
} // namespace Resources
