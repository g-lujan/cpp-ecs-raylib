#pragma once

#include <functional>
#include <raylib.h>
#include <unordered_map>
#include <unordered_set>
#include "../utils/side.hpp"
#include "../components/kinematics.hpp"

struct Controls : public Component {
  using MOVE_ACTION = std::function<void(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)>;
  std::unordered_map<KeyboardKey, MOVE_ACTION> key_to_movement;
  
  Controls();
  
  virtual std::string type_name() const { return "Controls"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }
};
