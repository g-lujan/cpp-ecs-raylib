#include "resources.hpp"

#include <utility>

namespace Resources {
  Manager::Manager()
  {
    _textures.insert(std::make_pair("empty", Graphics::Texture()));
    _textures.emplace("player", Paths::PLAYER_RED);
    _textures.emplace("hometown", Paths::HOMETOWN_IMAGE);
    _animations.emplace(std::make_pair(
        "player",
        std::unordered_map<KeyboardKey, Graphics::Animation_Settings>(
            {std::make_pair(KEY_NULL, Graphics::Animation_Settings(Vector2{0, 0}, 32, 3)),
             std::make_pair(KEY_RIGHT, Graphics::Animation_Settings(Vector2{0, 32}, 32, 8)),
             std::make_pair(KEY_LEFT, Graphics::Animation_Settings(Vector2{0, 32}, 32, 8, true))}
        )
    ));
    _tilemaps_paths.emplace("hometown", Paths::HOMETOWN_JSON);
  }

  Manager &get_resource_manager()
  {
    static Manager manager;
    return manager;
  }
} // namespace Resources
