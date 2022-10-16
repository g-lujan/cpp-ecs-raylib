#include "setup.hpp"
#include "../external/json.hpp"
#include "settings.hpp"
#include <fstream>
#include <string>
#include <vector>

namespace Setup {
  void players(ECS &ecs)
  {
    ecs.spawn_entity(
        Body({::floorf(Settings::SCREEN_WIDTH / 4), ::floorf(3 * Settings::SCREEN_HEIGHT / 4), 32, 32}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View(
            {{Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, {Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, 0.f, 1.f},
            WHITE,
            true,
            Resources::Map::HOMETOWN),
        Input(true));

    ecs.spawn_entity(
        Body({3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4, 32, 32}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View(
            {{3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             {3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             0.f,
             1.f},
            {0, 0, 255, 255},
            true,
            Resources::Map::HOMETOWN),
        Input(false));
  }

} // namespace Setup
