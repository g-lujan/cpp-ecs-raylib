#ifndef _GRAPHICS_H__
#define _GRAPHICS_H__

#include "raylib.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Graphics {
  class Texture {
  public:
    Texture() {}
    Texture(const std::string path_to_tex) { _tex = LoadTexture(path_to_tex.c_str()); }
    ~Texture() { UnloadTexture(_tex); }
    int &width() { return _tex.width; }
    int &height() { return _tex.height; }

    void draw(Rectangle src_rect, Vector2 dest, Color tint, bool flip = false);

  private:
    Texture2D _tex{};
  };

  struct Animation_Settings {
    Animation_Settings() {}
    Animation_Settings(Vector2 start_frame_pos, float step, int fps, bool flip = false)
        : start_frame_pos{start_frame_pos}, curr_frame{start_frame_pos.x, start_frame_pos.y, 32, 32}, step{step}, fps{fps}, flip{flip}
    {
    }

    Rectangle curr_frame;
    Vector2 start_frame_pos;
    /* the x step of the animation on the texture */
    float step;
    int fps;
    int frames{4};
    /*
     * frames_counter: Counts the amount of game frames elapsed since last animation frame.
     * If Game FPS is 60 and our anim FPS is 8, than after every 60/8 Game Frames
     * we need to increment the current_frame to the next
     */
    int frames_counter = 0;
    bool flip;
  };

  void step(Animation_Settings &settings);

} // namespace Graphics

#endif