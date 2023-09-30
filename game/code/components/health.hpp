#pragma once

#include "components.hpp"

struct Health : public Component {
  Health(int curr, int max) : curr{curr}, max{max} { serializable = true; }

  virtual std::string type_name() const { return "Health"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  int curr;
  int max;
};
