#ifndef _SETTINGS_H__
#define _SETTINGS_H__

#include "components.hpp"
#include "raylib.h"

namespace Settings {
  extern std::unordered_map<KeyboardKey, Movement> key_to_movement;
  extern std::function<Rectangle(const Rectangle &, const Movement &)> move;
} // namespace Settings

#endif
