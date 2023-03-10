#pragma once

#include <unordered_set>
#include "../ecs_core/components.hpp"
#include "../utils/side.hpp"
#include "../management/serialization.hpp"

enum class Body_Type {
  Player,
  Enemy,
  Door,
  Wall,
  Bullet,
  Sprite, // to delete when position and body decoupled
};


struct Collider : public Component {
  Collider(const Rectangle rect, const Body_Type type, bool kinematic) : bound{rect}, type{type}, rot{0.f}, kinematic{kinematic} {}
  Rectangle bound;
  Body_Type type;
  float rot;
  bool kinematic;
  std::unordered_set<Side> collision_sides;

  virtual std::string type_name() const { return "Collider"; }

  virtual nlohmann::json serialize()
  {
    nlohmann::json j = *this;
    return j;
  }
};
