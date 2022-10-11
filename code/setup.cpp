#include "setup.hpp"
#include "settings.hpp"

namespace Setup {
  void players(ECS &ecs)
  {
    ecs.spawn_entity(
        Body({Settings::SCREEN_WIDTH / 4, Settings::SCREEN_HEIGHT / 2, 16, 16}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View(
            {{Settings::SCREEN_WIDTH / 4, Settings::SCREEN_HEIGHT / 2},
             {Settings::SCREEN_WIDTH / 4, Settings::SCREEN_HEIGHT / 2},
             0.f,
             1.f},
            {255, 0, 0, 255},
            true),
        Input(true));

    ecs.spawn_entity(
        Body({3 * Settings::SCREEN_WIDTH / 4, Settings::SCREEN_HEIGHT / 2, 16, 16}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View(
            {{3 * Settings::SCREEN_WIDTH / 4, Settings::SCREEN_HEIGHT / 2}, {3 * Settings::SCREEN_WIDTH / 4, Settings::SCREEN_HEIGHT / 2}, 0.f, 1.f},
            {0, 0, 255, 255},
            true),
        Input(false));
  }
} // namespace Setup
