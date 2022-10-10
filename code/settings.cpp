#include "settings.hpp"

namespace Settings {
  const float STEP = 1.0f;

  std::unordered_map<KeyboardKey, Movement> key_to_movement{
      {KEY_NULL, {Movement_Type::IDLE, false}},
      {KEY_RIGHT, {Movement_Type::WALK, false}},
      {KEY_LEFT, {Movement_Type::WALK, true}},
  };

  std::function<Rectangle(const Rectangle &, const Movement &)> move = [](const Rectangle &rect, const Movement &movement) {
    if (movement.type == Movement_Type::IDLE) {
      return rect;
    }
    float dir = movement.flip ? -1 : 1;
    return (Rectangle){rect.x + (STEP * dir), rect.y, rect.width, rect.height};
  };
} // namespace Settings
