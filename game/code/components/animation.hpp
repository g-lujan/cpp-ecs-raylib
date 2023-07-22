#pragma once

#include "components.hpp"
#include "../graphics/graphics.hpp"
#include "../management/serialization.hpp"

struct Anim : public Component {
  Anim(const std::string name, Graphics::Texture *tex, Graphics::Frame &settings) : name{name}, tex{tex}, settings{settings} {}

  virtual std::string type_name() const { return "Anim"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  Graphics::Texture *tex{nullptr};
  Graphics::Frame settings;
  Graphics::Flip flip;
  std::string name;
};