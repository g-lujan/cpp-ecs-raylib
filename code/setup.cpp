#include "setup.hpp"

namespace Setup {
  unsigned long long players(ECS &ecs)
  {
    int screenWidth = 800;
    int screenHeight = 450;
    auto player_one = ecs.spawn_entity(
        Body({screenWidth / 4, screenHeight / 2, 16, 16}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View({{screenWidth / 4 - 8, screenHeight / 2 - 8}, {screenWidth / 4 - 8, screenHeight / 2 - 8}, 0.f, 1.f}, {255, 0, 0, 255}, true),
        Input(true));
    ecs.spawn_entity(
        Body({3 * screenWidth / 4, screenHeight / 2, 16, 16}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View({{3 * screenWidth / 4, screenHeight / 2}, {3 * screenWidth / 4, screenHeight / 2}, 0.f, 1.f}, {0, 0, 255, 255}, true),
        Input(false));
    return player_one;
  }
} // namespace Setup
