#pragma once

#include "../ecs_core/components.hpp"
#include "../graphics/graphics.hpp"
#include "../management/serialization.hpp"

struct Anim : public Component {
  Anim(const std::string name, Graphics::Texture *tex, Graphics::Frame &settings) : name{name}, tex{tex}, settings{settings} {}
  Graphics::Texture *tex{nullptr};
  Graphics::Frame settings;
  Graphics::Flip flip;
  std::string name;

  virtual std::string type_name() const { return "Anim"; }

  virtual nlohmann::json serialize()
  {
    nlohmann::json j = *this;
    return j;
  }
};