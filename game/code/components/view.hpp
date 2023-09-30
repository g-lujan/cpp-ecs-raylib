#pragma once

#include "components.hpp"
#include "../management/serialization.hpp"

struct View : public Component {
  View(Camera2D cam, Color tint, bool active, const std::string &map) : camera{cam}, tint{tint}, active{active}, map{map} {}

  virtual std::string type_name() const { return "View"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  Camera2D camera{0};
  Color tint;
  std::string map;
  bool active{false};
};