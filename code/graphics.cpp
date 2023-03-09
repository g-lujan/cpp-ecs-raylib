#include "graphics.hpp"
#include "settings.hpp"
#include "../external/json.hpp"
#include "raylib.h"
#include "settings.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace Graphics {

  void Texture::draw(Rectangle src_rect, Vector2 dest, Color tint, float rotation, Flip flip)
  {
    if (flip.horizontally || flip.vertically) {
      DrawTexturePro(_tex,
                     {src_rect.x, src_rect.y, (flip.horizontally ? -1 : 1) * src_rect.width, (flip.vertically ? -1 : 1) * src_rect.height},
                     {dest.x, dest.y, Settings::TILE_SIZE, Settings::TILE_SIZE},
                     {Settings::TILE_SIZE / 2, Settings::TILE_SIZE / 2},
                     rotation,
                     tint);
    }
    else {
      DrawTexturePro(_tex,
                     src_rect,
                     {dest.x, dest.y, Settings::TILE_SIZE, Settings::TILE_SIZE},
                     {Settings::TILE_SIZE / 2, Settings::TILE_SIZE / 2},
                     rotation,
                     tint);
    }
  }

  Frame::Frame(std::string name, Vector2 start_frame_pos, int fps)
      : action{name},
        start_frame_pos{start_frame_pos},
        curr_frame{start_frame_pos.x, start_frame_pos.y, Settings::TILE_SIZE, Settings::TILE_SIZE},
        fps{fps},
        step{Settings::TILE_SIZE}
  {
  }

  void step(Frame &settings)
  {
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