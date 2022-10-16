#include "setup.hpp"
#include "../external/json.hpp"
#include "settings.hpp"
#include <string>
#include <vector>

namespace Setup {
  void players(ECS &ecs, Resources::Manager &resources_manager)
  {
    ecs.spawn_entity(
        Collider({::floorf(Settings::SCREEN_WIDTH / 4), ::floorf(3 * Settings::SCREEN_HEIGHT / 4), 32, 32}, Body_Type::Player),
        Health(100, 100),
        Anim("player", &resources_manager.texture("player"), resources_manager.animation("player", KEY_NULL)),
        View(
            {{Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, {Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, 0.f, 1.f},
            WHITE,
            true,
            "hometown"
        ),
        Input(true));

    ecs.spawn_entity(
        Collider({3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4, 32, 32}, Body_Type::Player),
        Health(100, 100),
        Anim("player", &resources_manager.texture("player"), resources_manager.animation("player", KEY_NULL)),
        View(
            {{3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             {3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             0.f,
             1.f},
            {0, 0, 255, 255},
            true,
            "hometown"
        ),
        Input(false));
  }

} // namespace Setup
