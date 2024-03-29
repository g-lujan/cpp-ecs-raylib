#pragma once

#include "../management/serialization.hpp"
#include "components.hpp"
#include "raylib.h"

struct Kinematics : public Component {
  Kinematics(Vector2 position) : position{position} {}

  virtual std::string type_name() const { return "Kinematics"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  Vector2 position;
  Vector2 velocity = {0.f, 0.f};
  Vector2 acceleration = {0.f, 0.f}; // useful for jumping, for ex
  Vector2 orientation = {1.f, 0.f};
};