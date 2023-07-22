#ifndef _GRAPHICS_H__
#define _GRAPHICS_H__

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace Graphics {
  struct Flip {
    bool horizontally = false;
    bool vertically = false;
  };

  class Texture {
  public:
    Texture() {}
    Texture(const std::string path_to_tex);
    ~Texture() { UnloadTexture(_tex); }
    void draw(Rectangle src_rect, Vector2 dest, Color tint, float rotation, Flip flip);
    int &width() { return _tex.width; }
    int &height() { return _tex.height; }

  private:
    Texture2D _tex{};
  };

  struct Frame {
    Frame() {}
    Frame(std::string name, Vector2 start_frame_pos, int fps);
    void step();

    std::string action;
    Rectangle curr_frame;
    Vector2 start_frame_pos;

    int step_size; // the x step of the animation on the texture
    int fps;
    int frames{4};

    /*
     * frames_counter: Counts the amount of game frames elapsed since last animation frame.
     * If Game FPS is 60 and our anim FPS is 8, than after every 60/8 Game Frames
     * we need to increment the current_frame to the next
     */
    int frames_counter = 0;
  };
} // namespace Graphics

#endif