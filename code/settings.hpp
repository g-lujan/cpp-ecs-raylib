#ifndef _SETTINGS_H__
#define _SETTINGS_H__

#include "components.hpp"
#include "raylib.h"

namespace Settings {
  const float STEP = 1.0f;
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 450;
  //const int FPS = 60;// introduces circular dependency -> better put pure constants in another file

  const std::unordered_map<KeyboardKey, Movement> key_to_movement{
      {KEY_NULL, {Movement_Type::IDLE, false}},
      {KEY_RIGHT, {Movement_Type::WALK, false}},
      {KEY_LEFT, {Movement_Type::WALK, true}},
  };

  const std::function<Rectangle(const Rectangle &, const Movement &)> move = [](const Rectangle &rect, const Movement &movement) {
    if (movement.type == Movement_Type::IDLE) {
      return rect;
    }
    float dir = movement.flip ? -1 : 1;
    return (Rectangle){rect.x + (STEP * dir), rect.y, rect.width, rect.height};
  };
  /*
  extern std::unordered_map<KeyboardKey, Movement> key_to_movement;
  extern std::function<Rectangle(const Rectangle &, const Movement &)> move;
  */
} // namespace Settings

#endif
