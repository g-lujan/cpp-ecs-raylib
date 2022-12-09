#pragma once
#include "components.hpp"
#include <algorithm>
#include <functional>
#include <raylib.h>
#include <unordered_map>

struct Movement {
  bool flip{false};
  bool move;
  bool crouch;
  bool jump;
};

const std::unordered_map<KeyboardKey, Movement> key_to_movement{
    {KEY_NULL, {.flip = false, .move = false, .crouch = false, .jump = false}},
    {KEY_RIGHT, {.flip = false, .move = true, .crouch = false, .jump = false}},
    {KEY_LEFT, {.flip = true, .move = true, .crouch = false, .jump = false}},
    {KEY_SPACE, {.flip = false, .move = false, .crouch = false, .jump = true}},
};

const std::function<Side(const Rectangle &rect, const Rectangle &other)> get_collision_side = [](const Rectangle &rect, const Rectangle &other) {
  int amtRight = (rect.x + rect.width) - other.x;
  int amtLeft = (other.x + other.width) - rect.x;
  int amtTop = (other.y + other.height) - rect.y;
  int amtBottom = (rect.y + rect.height) - other.y;

  std::vector<int> vals = {abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom)};
  int lowest = *std::min_element(vals.begin(), vals.end());

  return lowest == abs(amtRight)    ? Side::RIGHT
         : lowest == abs(amtLeft)   ? Side::LEFT
         : lowest == abs(amtTop)    ? Side::TOP
         : lowest == abs(amtBottom) ? Side::BOTTON
                                    : Side::NONE;
};
