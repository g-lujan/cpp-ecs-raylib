#ifndef _SETTINGS_H__
#define _SETTINGS_H__

#include "raylib.h"

namespace Settings {
  inline constexpr float STEP = 1.0f;
  inline constexpr int SCREEN_WIDTH = 800;
  inline constexpr int SCREEN_HEIGHT = 600;
  const int FPS = 60;

  enum class Movement_Type {
    IDLE,
    WALK,
    JUMPING,
  };

  // Not a component
  struct Movement {
    Movement_Type type{Movement_Type::IDLE};
    bool flip{false};
  };

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
    return Rectangle{rect.x + (STEP * dir), rect.y, rect.width, rect.height};
  };
} // namespace Settings

#endif
