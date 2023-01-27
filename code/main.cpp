#include "ecs.hpp"
#include "raylib.h"
#include "resources.hpp"
#include "settings.hpp"
#include "tilemap.hpp"

int main(void)
{
  InitWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, Settings::TITLE);
  SetTargetFPS(Settings::FPS);
  ECS ecs;
  bool loaded_map = Tilemap::load(ecs, "hometown");
  while (!WindowShouldClose() && loaded_map)
  {
    ecs.run_system<System::Input>();
    ecs.run_system<System::Player_Movement>();
    ecs.run_system<System::Physics>();
    ecs.run_system<System::Draw>();
  }
  CloseWindow();
  return 0;
}
