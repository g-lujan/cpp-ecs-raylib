#include "controls.hpp"

void idle(Kinematics &kinematics, std::unordered_set<Side> &collision_sides) {}

void move_right(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)
{
  if (!collision_sides.contains(Side::RIGHT)) {
    kinematics.velocity.x = 50;
    kinematics.orientation.x = 1.f;
    kinematics.orientation.y = 0.f;
  }
}

void move_left(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)
{
  if (!collision_sides.contains(Side::LEFT)) {
    kinematics.velocity.x = -50;
    kinematics.orientation.x = -1.f;
    kinematics.orientation.y = 0.f;
  }
}

void move_up(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)
{
  if (!collision_sides.contains(Side::TOP)) {
    kinematics.velocity.y = -50;
    kinematics.orientation.x = 0.f;
    kinematics.orientation.y = -1.f;
  }
}

void move_down(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)
{
  if (!collision_sides.contains(Side::BOTTON)) {
    kinematics.velocity.y = 50;
    kinematics.orientation.x = 0.f;
    kinematics.orientation.y = 1.f;
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
  key_to_movement[KEY_RIGHT] = Move_Action("move_right", move_right);
  key_to_movement[KEY_D] = Move_Action("move_right", move_right);
  key_to_movement[KEY_LEFT] = Move_Action("move_left", move_left);
  key_to_movement[KEY_A] = Move_Action("move_left", move_left);
  key_to_movement[KEY_W] = Move_Action("move_up", move_up);
  key_to_movement[KEY_UP] = Move_Action("move_up", move_up);
  key_to_movement[KEY_S] = Move_Action("move_down", move_down);
  key_to_movement[KEY_DOWN] = Move_Action("move_down", move_down);
}