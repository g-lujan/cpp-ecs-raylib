#pragma once

#include <functional>
#include <raylib.h>
#include <unordered_map>
#include <unordered_set>
#include "components.hpp"
#include "../utils/side.hpp"
#include "../components/kinematics.hpp"

struct Move_Action {
  Move_Action() {}
  Move_Action(std::string id, std::function<void(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)> act) : 
      action_id{id}, action{act}
  {}

  std::string action_id;
  std::function<void(Kinematics &kinematics, std::unordered_set<Side> &collision_sides)> action;
};

struct Controls : public Component {
  Controls();
  
  virtual std::string type_name() const { return "Controls"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  std::unordered_map<KeyboardKey, Move_Action> key_to_movement;
};
