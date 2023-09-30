#pragma once

#include "../management/serialization.hpp"
#include "components.hpp"

struct AI : public Component {
  AI(bool active) : active{active} {}

  virtual std::string type_name() const { return "AI"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  bool active = false;
  float last_call = 0.f;
  KeyboardKey last_action = KEY_NULL;
};
