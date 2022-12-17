#include "movement.hpp"

void step_kinematics(Kinematics &kinematics)
{
  const float dt = GetFrameTime();
  kinematics.position.x += kinematics.velocity.x * dt + (kinematics.acceleration.x * dt * dt) / 2;
  kinematics.velocity.x += kinematics.acceleration.x * dt;
  kinematics.position.y += kinematics.velocity.y * dt + (kinematics.acceleration.y * dt * dt) / 2;
  kinematics.velocity.y += kinematics.acceleration.y * dt;
};

Side get_collision_side(const Rectangle &rect, const Rectangle &other)
{
  if (!CheckCollisionRecs(rect, other)) {
    return Side::NONE;
  }
  int amtRight = (rect.x + rect.width) - other.x;
  int amtLeft = (other.x + other.width) - rect.x;
  int amtTop = (other.y + other.height) - rect.y;
  int amtBottom = (rect.y + rect.height) - other.y;

  std::vector<int> vals = {abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom)};
  int lowest = *std::min_element(vals.begin(), vals.end());

  return lowest == abs(amtRight) ? Side::RIGHT : lowest == abs(amtLeft) ? Side::LEFT : lowest == abs(amtTop) ? Side::TOP : Side::BOTTON;
};

void idle(Kinematics &kinematics, std::unordered_set<Side> &collision_sides) {}

void move_right(Kinematics& kinematics, std::unordered_set<Side>& collision_sides) {
  if (!collision_sides.contains(Side::RIGHT)) {
    kinematics.velocity.x = 50;
  }
}

void move_left(Kinematics& kinematics, std::unordered_set<Side>& collision_sides) {
  if (!collision_sides.contains(Side::LEFT)) {
    kinematics.velocity.x = -50;
  }
}

void jump(Kinematics& kinematics, std::unordered_set<Side>& collision_sides) {
  if (collision_sides.contains(Side::BOTTON)) {
    kinematics.acceleration.y = -3000;
  }
}