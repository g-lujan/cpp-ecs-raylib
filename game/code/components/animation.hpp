#pragma once

#include "../graphics/graphics.hpp"
#include "../management/serialization.hpp"
#include "components.hpp"

struct Anim : public Component {
  Anim(const std::string &name, const std::string &tex, Graphics::Frame &frame) : name{name}, texture_id{tex}, frame{frame} {}

  virtual std::string type_name() const { return "Anim"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  std::string texture_id;
  Graphics::Frame frame;
  Graphics::Flip flip;
  std::string name;
};