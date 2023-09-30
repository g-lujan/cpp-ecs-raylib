#pragma once

#include "components.hpp"

struct Input : public Component {
  Input(bool active) : active{active} {}

  virtual std::string type_name() const { return "Input"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  bool active;
  std::vector<KeyboardKey> keys_pressed;
  bool changed{true};
};
