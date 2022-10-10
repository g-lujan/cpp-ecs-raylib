#include "ecs.hpp"
#include "raylib.h"
#include "resources.hpp"
#include "setup.hpp"
#include <functional>
#include <iostream>
#include <stdlib.h>

int main(void)
{
  int screenWidth = 800;
  int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "Shadows");
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  // init resources
  Resources::textures.emplace("player", "resources/images/red.png");
  Resources::animations = {{
      {KEY_NULL, Resources::Animation(&Resources::textures["player"], Resources::Animation_Type::PLAYER, {0, 0}, 32, 3)},
      {KEY_RIGHT, Resources::Animation(&Resources::textures["player"], Resources::Animation_Type::PLAYER, {0, 32}, 32, 8)},
      {KEY_LEFT, Resources::Animation(&Resources::textures["player"], Resources::Animation_Type::PLAYER, {0, 32}, 32, 8, true)},
  }};
  ECS ecs;
  // Player
  Setup::players(ecs);
  // wall
  // to be Setup::map(...)
  ecs.spawn_entity(Body({screenWidth / 2 + 100, screenHeight / 2 + 100, 16, 16}, Body_Type::Wall), Anim(Resources::Animation_Type::PLAYER));
  ecs.spawn_entity(Body({screenWidth / 2 - 100, screenHeight / 2 - 200, 16, 16}, Body_Type::Wall), Anim(Resources::Animation_Type::PLAYER));
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // systems executions
    ecs.run_system<System::Input>();
    ecs.run_system<System::Physics>();
    ecs.run_system<System::Animation>();
  }
  CloseWindow(); // Close window and OpenGL context
  return 0;
}
