#include "movement.hpp"
#include "../components/kinematics.hpp"

using std::min;

constexpr float EPSILON_COLLISION = 0.1f;

/*
 * +------------------+                  +
 * |                  |                  |
 * |                  |                  |
 * |      rect  d1    |      d2          |
 * |        +---------x-------------+    |
 * |     d3 |         |             |    | d4
 * +--------x---------+             |    |
 *          |      other            |    |
 *          |                       |    |
 *          |                       |    |
 *          |                       |    |
 *          +-----------------------+    +
 */

Side get_collision_side(const Rectangle &rect, const Rectangle &other)
{
  if (!CheckCollisionRecs(rect, other)) {
    return Side::NONE;
  }
  const float distance_from_rect_right_border_to_other_left_border = abs((rect.x + rect.width) - other.x);  // d1
  const float distance_from_other_right_border_to_rect_left_border = abs((other.x + other.width) - rect.x); // d2
  const float min_distance_right_to_left =
      min(distance_from_rect_right_border_to_other_left_border, distance_from_other_right_border_to_rect_left_border);

  const float distance_from_rect_lower_border_to_other_top_border = abs((rect.y + rect.height) - other.y);  // d3
  const float distance_from_other_lower_border_to_rect_top_border = abs((other.y + other.height) - rect.y); // d4
  const float min_distance_lower_to_top =
      min(distance_from_other_lower_border_to_rect_top_border, distance_from_rect_lower_border_to_other_top_border);

  const float min_distance = min(min_distance_right_to_left, min_distance_lower_to_top);

  return std::abs(min_distance - distance_from_rect_right_border_to_other_left_border) < EPSILON_COLLISION   ? Side::RIGHT
         : std::abs(min_distance - distance_from_other_right_border_to_rect_left_border) < EPSILON_COLLISION ? Side::LEFT
         : std::abs(min_distance - distance_from_other_lower_border_to_rect_top_border) < EPSILON_COLLISION  ? Side::TOP
                                                                                                             : Side::BOTTON;
};