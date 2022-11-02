#ifndef _SETTINGS_H__
#define _SETTINGS_H__

#include "raylib.h"
#include <functional>

namespace Settings {
  inline constexpr float STEP = 1.0f;
  inline constexpr int SCREEN_WIDTH = 800;
  inline constexpr int SCREEN_HEIGHT = 600;
  inline constexpr int FPS = 60;
  inline constexpr auto TITLE = "Shadow";
  inline constexpr int TILE_SIZE = 32;
  inline constexpr float DEFAULT_ZOOM = 1.5f;
} // namespace Settings

#endif
