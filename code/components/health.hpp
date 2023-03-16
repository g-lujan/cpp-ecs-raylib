#pragma once

#include "../ecs_core/components.hpp"

struct Health : public Component {
  Health(int curr, int max) : curr{curr}, max{max} { serializable = true; }

  int curr;
  int max;
  virtual std::string type_name() const { return "Health"; }
  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }
};
