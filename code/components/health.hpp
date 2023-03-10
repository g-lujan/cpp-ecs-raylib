#pragma once

#include "../ecs_core/components.hpp"

struct Health : public Component {
  Health(int curr, int max) : curr{curr}, max{max} { serializable = true; }

  int curr;
  int max;
  virtual std::string type_name() const { return "Health"; }
  virtual nlohmann::json serialize()
  {
    nlohmann::json j = *this;
    return j;
  }
};
