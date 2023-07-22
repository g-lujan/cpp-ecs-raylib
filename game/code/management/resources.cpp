#include "resources.hpp"
#include "../graphics/graphics.hpp"

#include <utility>

namespace Resources {
  Manager::Manager()
  {
    _textures.insert(std::make_pair("empty", Graphics::Texture()));
    _textures.emplace("player", Paths::PLAYER_RED);
    _textures.emplace("hometown", Paths::HOMETOWN_IMAGE);
    _animations.emplace(std::make_pair(
        "player",
        std::unordered_map<KeyboardKey, Graphics::Frame>(
            {
                std::make_pair(KEY_NULL, Graphics::Frame("idle", Vector2{0, 0}, 3)),
                std::make_pair(KEY_RIGHT, Graphics::Frame("move_right", Vector2{0, 32}, 8)),
                std::make_pair(KEY_LEFT, Graphics::Frame("move_left", Vector2{0, 32}, 8)),
                std::make_pair(KEY_SPACE, Graphics::Frame("jump", Vector2{0, 64}, 4))
            }
        )
    ));
    _tilemaps_paths.emplace("hometown", Paths::HOMETOWN_JSON);
  }

  Graphics::Texture &Manager::texture(const std::string tex_name) { 
      return _textures[tex_name];
  }
  
  Graphics::Frame &Manager::animation(const std::string anim_name, const KeyboardKey key) { 
      return _animations[anim_name][key];
  }

  std::string Manager::tilemap_path(const std::string map_name) { 
      return _tilemaps_paths[map_name]; 
  }

  Manager &get_resource_manager()
  {
    static Manager manager;
    return manager;
  }
} // namespace Resources
