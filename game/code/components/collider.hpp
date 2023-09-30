#pragma once

#include "../management/serialization.hpp"
#include "../utils/side.hpp"
#include "components.hpp"
#include <unordered_set>
#include "raylib.h"

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

  virtual std::string type_name() const { return "Collider"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  Rectangle bound;
  Body_Type type;
  float rot;
  bool kinematic;
  std::unordered_set<Side> collision_sides;
};
