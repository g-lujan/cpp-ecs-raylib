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
    kinematics.acceleration.y = -3500;
  }
}

Controls::Controls()
{
  // defaults
  key_to_movement[KEY_NULL] = Move_Action("idle", idle);
  key_to_movement[KEY_ENTER] = Move_Action("idle", idle);
  key_to_movement[KEY_RIGHT] = Move_Action("move_right", move_right);
  key_to_movement[KEY_D] = Move_Action("move_right", move_right);
  key_to_movement[KEY_LEFT] = Move_Action("move_left", move_left);
  key_to_movement[KEY_A] = Move_Action("move_left", move_left);
  key_to_movement[KEY_SPACE] = Move_Action("jump", jump);
  key_to_movement[KEY_W] = Move_Action("jump", jump);
}