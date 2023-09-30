#include "ecs.hpp"
#include "graphics/tilemap.hpp"
#include "management/resources.hpp"
#include "management/settings.hpp"
#include "raylib.h"
#include "systems/available_systems.hpp"
#include "utils/time.hpp"

int main(void)
{
  InitWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, Settings::TITLE);
  SetTargetFPS(Settings::FPS);
  ECS ecs;
  bool loaded_map = Tilemap::load(ecs, "hometown");
  while (!WindowShouldClose() && loaded_map) {
    cap_framerate();
    // order matters
    ecs.run_system<System::Input>();
    ecs.run_system<System::AI>();
    ecs.run_system<System::InGameMenu>();
    ecs.run_system<System::Player_Movement>();
    ecs.run_system<System::Physics>();
    ecs.run_system<System::Draw>();
  }
  CloseWindow();
  return 0;
}
