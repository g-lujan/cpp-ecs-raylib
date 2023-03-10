#pragma once

#include "../ecs_core/components.hpp"
#include "../management/serialization.hpp"

struct AI : public Component {
  AI(bool active) : active{active} {}
  bool active = false;
  float last_call = 0.f;
  KeyboardKey last_action = KEY_NULL;

  virtual std::string type_name() const { return "AI"; }

  virtual nlohmann::json serialize()
  {
    nlohmann::json j = *this;
    return j;
  }
};
