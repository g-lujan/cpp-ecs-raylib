#include "ecs.hpp"
#include "raylib.h"
#include "resources.hpp"
#include "settings.hpp"
#include "setup.hpp"
#include <functional>
#include <iostream>
#include <stdlib.h>
#include "map.hpp"

void call_drawing_systems(ECS &ecs)
{
  BeginDrawing();
  ClearBackground(BLACK);
  ecs.run_system<System::Tile>();
  ecs.run_system<System::Animation>();
  EndDrawing();
}

int main(void)
{
  InitWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, "Shadows");
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  // init resources
  Resources::textures.emplace("player", "resources/images/red.png");
  Resources::animations = {{
      {KEY_NULL, Resources::Animation(&Resources::textures["player"], Resources::Animation_Type::PLAYER, {0, 0}, 32, 3)},
      {KEY_RIGHT, Resources::Animation(&Resources::textures["player"], Resources::Animation_Type::PLAYER, {0, 32}, 32, 8)},
      {KEY_LEFT, Resources::Animation(&Resources::textures["player"], Resources::Animation_Type::PLAYER, {0, 32}, 32, 8, true)},
  }};
  ECS ecs;
  Map map(Resources::Map::HOMETOWN);
  map.generate_components(ecs);
  Setup::players(ecs);
  while (!WindowShouldClose() && map.active()) // Detect window close button or ESC key
  {
    // systems executions
    ecs.run_system<System::Input>();
    ecs.run_system<System::Physics>();
    call_drawing_systems(ecs);
  }
  CloseWindow(); // Close window and OpenGL context
  return 0;
}
