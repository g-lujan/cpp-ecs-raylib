#pragma once

#include "../ecs_core/components.hpp"

struct Input : public Component {
  Input(bool active) : active{active} {}
  bool active;
  std::vector<KeyboardKey> keys_pressed;
  bool changed{true};
  virtual std::string type_name() const { return "Input"; }
  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }
};
