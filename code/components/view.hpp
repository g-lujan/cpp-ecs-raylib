#pragma once

#include "../ecs_core/components.hpp"
#include "../management/serialization.hpp"

struct View : public Component {
  View(Camera2D cam, Color tint, bool active, std::string map) : camera{cam}, tint{tint}, active{active}, map{map} {}
  Camera2D camera{0};
  Color tint;
  std::string map;
  bool active{false};

  virtual std::string type_name() const { return "View"; }

  virtual nlohmann::json serialize()
  {
    nlohmann::json j = *this;
    return j;
  }
};