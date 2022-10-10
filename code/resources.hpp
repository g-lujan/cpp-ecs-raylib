#ifndef _RESOURCES_H__
#define _RESOURCES_H__

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace Resources {
  class Texture {
  public:
    Texture() {}
    Texture(const std::string path_to_tex) { _tex = LoadTexture(path_to_tex.c_str()); }
    ~Texture() { UnloadTexture(_tex); }
    int &width() { return _tex.width; }
    int &height() { return _tex.height; }

    void draw(Rectangle src_rect, Vector2 dest, Color tint, bool flip)
    {
      if (flip) {
        DrawTexturePro(_tex, {src_rect.x, src_rect.y, -src_rect.width, src_rect.height}, {dest.x, dest.y, 32 * scale, 32 * scale}, {0, 0}, 0, tint);
      }
      else {
        DrawTexturePro(_tex, src_rect, {dest.x, dest.y, 32 * scale, 32 * scale}, {0, 0}, 0, tint);
      }
    }

  private:
    Texture2D _tex;
    int scale{2};
  };

  enum Animation_Type {
    PLAYER,
  };

  class Animation {
  public:
    Animation() {}
    Animation(Texture *tex, Animation_Type type, Vector2 start_frame_pos, float step, int fps, bool flip = false)
        : _tex{tex},
          _type{type},
          _start_frame_pos{start_frame_pos},
          _curr_frame{start_frame_pos.x, start_frame_pos.y, 32, 32},
          _step{step},
          _fps{fps},
          _flip{flip}
    {
    }

    void step_animation()
    {
      _frames_counter++;
      // TODO passar 60 pra um global GAME_FPS
      if (_frames_counter >= (60 / _fps)) {
        _frames_counter = 0;
	    std::cout << 1 << std::endl;
        if (_curr_frame.x > _tex->width()) {
          // rewind
	    std::cout << 2 << std::endl;
          _curr_frame.x = _start_frame_pos.x;
        }
	    std::cout << 3 << std::endl;
        _curr_frame.x += _step;
      }
    }

    void run(Vector2 dest, Color tint)
    {
      step_animation();
      _tex->draw(_curr_frame, dest, tint, _flip);
    }

    Animation_Type type() { return _type; }

  private:
    Texture *_tex{nullptr};
    Rectangle _curr_frame;
    Vector2 _start_frame_pos;
    Animation_Type _type;
    /* the x step of the animation on the texture */
    float _step;
    int _fps;
    /*
     * frames_counter: Counts the amount of game frames elapsed since last animation frame.
     * If Game FPS is 60 and our anim FPS is 8, than after every 60/8 Game Frames
     * we need to increment the current_frame to the next
     */
    int _frames_counter = 0;
    bool _flip;
  };

  // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
  extern std::unordered_map<std::string, Texture> textures;
  extern std::vector<std::unordered_map<KeyboardKey, Animation>> animations;
} // namespace Resources

#endif
