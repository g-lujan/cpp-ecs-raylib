#pragma once

#include "../ecs_core/components.hpp"

struct Player : public Component {
  Player() { serializable = true; }

  virtual std::string type_name() const { return "Player"; }

  virtual nlohmann::json serialize()
  {
    nlohmann::json j = *this;
    return j;
  }
};
