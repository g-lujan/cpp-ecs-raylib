#pragma once
#include <algorithm>
#include <functional>
#include <raylib.h>
#include <unordered_map>
#include <unordered_set>
#include "utils/side.hpp"

struct Kinematics;

void step_kinematics(Kinematics &kinematics);
Side get_collision_side(const Rectangle &rect, const Rectangle &other);
void idle(Kinematics &kinematics, std::unordered_set<Side> &collision_sides);
void move_right(Kinematics &kinematics, std::unordered_set<Side> &collision_sides);
void move_left(Kinematics &kinematics, std::unordered_set<Side> &collision_sides);
void jump(Kinematics &kinematics, std::unordered_set<Side> &collision_sides);

const std::unordered_map<KeyboardKey, std::function<void(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)>> key_to_movement{
    {KEY_NULL, idle},
    {KEY_ENTER, idle},
    {KEY_RIGHT, move_right},
    {KEY_LEFT, move_left}, 
    {KEY_SPACE, jump}
  };