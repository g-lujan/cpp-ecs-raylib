#include "controls.hpp"

void idle(Kinematics &kinematics, std::unordered_set<Side> &collision_sides) {}

void move_right(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)
{
  if (!collision_sides.contains(Side::RIGHT)) {
    kinematics.velocity.x = 50;
  }
}

void move_left(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)
{
  if (!collision_sides.contains(Side::LEFT)) {
    kinematics.velocity.x = -50;
  }
}

void jump(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)
{
  if (collision_sides.contains(Side::BOTTON)) {
    kinematics.acceleration.y = -4000;
  }
}

Controls::Controls()
{
  // defaults
  key_to_movement[KEY_NULL] = idle;
  key_to_movement[KEY_ENTER] = idle;
  key_to_movement[KEY_RIGHT] = move_right;
  key_to_movement[KEY_LEFT] = move_left;
  key_to_movement[KEY_SPACE] = jump;
}