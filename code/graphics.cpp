#include "graphics.hpp"
#include "raylib.h"
#include "../external/json.hpp"
#include <iostream>
#include <string>
#include <fstream>

namespace Graphics {

  void Texture::draw(Rectangle src_rect, Vector2 dest, Color tint, bool flip)
  {
    if (flip) {
      DrawTexturePro(_tex, {src_rect.x, src_rect.y, -src_rect.width, src_rect.height}, {dest.x, dest.y, 32, 32}, {0, 0}, 0, tint);
    }
    else {
      DrawTexturePro(_tex, src_rect, {dest.x, dest.y, 32, 32}, {0, 0}, 0, tint);
    }
  }

  void step(Animation_Settings& settings) {
    if (settings.fps == 0) {
      return;
    }
    settings.frames_counter++;
    if (settings.frames_counter >= (60 / settings.fps)) {
      settings.frames_counter = 0;
      if (settings.curr_frame.x > settings.frames * settings.step) {
        // rewind
        settings.curr_frame.x = settings.start_frame_pos.x;
      }
      settings.curr_frame.x += settings.step;
    }
  }
} // namespace Graphics