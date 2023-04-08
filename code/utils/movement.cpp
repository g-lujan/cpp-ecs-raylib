#include "movement.hpp"
#include "../components/kinematics.hpp"

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