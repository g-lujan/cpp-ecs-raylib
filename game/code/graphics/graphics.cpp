#include "graphics.hpp"
#include "raylib.h"
#include <fstream>
#include <iostream>
#include <string>

#include "../management/settings.hpp"
#include "../../external/json.hpp"
#include "../management/settings.hpp"

namespace Graphics {

  Texture::Texture(const std::string &path_to_tex) { _tex = LoadTexture(path_to_tex.c_str()); }

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

  Frame::Frame(const std::string &name, Vector2 start_pos, int fps)
      : action{name},
        start_pos{start_pos},
        src_rect{start_pos.x, start_pos.y, Settings::TILE_SIZE, Settings::TILE_SIZE},
        fps{fps},
        step_size{Settings::TILE_SIZE}
  {
  }

  void Frame::step(float rewind_offset)
  {
    if (fps == 0) {
      return;
    }
    frames_counter++;
    if (frames_counter >= (60 / fps)) {
      frames_counter = 0;
      if (src_rect.x > frames * step_size) {
        // rewind
        src_rect.x = start_pos.x + rewind_offset;
      }
      src_rect.x += step_size;
    }
  }
} // namespace Graphics