#include "setup.hpp"
#include "../external/json.hpp"
#include "settings.hpp"
#include <string>
#include <vector>

namespace Setup {
  void players(ECS &ecs, Resources::Manager &resources_manager)
  {
    ecs.spawn_entity(Position(::floorf(Settings::SCREEN_WIDTH / 4), ::floorf(3 * Settings::SCREEN_HEIGHT / 4)),
        Collider({::floorf(Settings::SCREEN_WIDTH / 4), ::floorf(3 * Settings::SCREEN_HEIGHT / 4), Settings::TILE_SIZE, Settings::TILE_SIZE},
                 Body_Type::Player, true),
        Health(100, 100),
        Anim("player", &resources_manager.texture("player"), resources_manager.animation("player", KEY_NULL)),
        View({{Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
              {Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
              0.f,
              Settings::DEFAULT_ZOOM
            },
            WHITE,
            true,
            "hometown"
        ),
        Input(true),
        Player());

    ecs.spawn_entity(
        Position(3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4),
        Collider({3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4, Settings::TILE_SIZE, Settings::TILE_SIZE}, Body_Type::Player, true),
        Health(100, 100),
        Anim("player", &resources_manager.texture("player"), resources_manager.animation("player", KEY_NULL)),
        View(
            {{3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             {3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             0.f,
             Settings::DEFAULT_ZOOM},
            {0, 0, 255, 255},
            true,
            "hometown"
        ),
        Input(false),
        Player());
  }

} // namespace Setup
