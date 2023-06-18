#pragma once

#include "components.hpp"
#include "../management/serialization.hpp"

struct Kinematics : public Component {
  Kinematics(Vector2 position) : position{position} {}
  Vector2 position;
  Vector2 velocity = {0.f, 0.f};
  Vector2 acceleration = {0.f, 0.f}; // useful for jumping, for ex
  virtual std::string type_name() const { return "Kinematics"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }
};