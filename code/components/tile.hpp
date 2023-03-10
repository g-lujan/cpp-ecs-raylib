#pragma once

#include "../ecs_core/components.hpp"
#include "../graphics/graphics.hpp"

struct Tile : public Component {
  Tile(Graphics::Texture *tex, Rectangle src_rect, Graphics::Flip flip, float rotation = 0.f)
      : tex{tex}, src_rect{src_rect}, rotation{rotation}, flip{flip}
  {
  }

  Graphics::Texture *tex{nullptr};
  Rectangle src_rect;
  float rotation;
  Graphics::Flip flip;
  virtual std::string type_name() const { return "Tile"; }
  virtual nlohmann::json serialize()
  {
    nlohmann::json j = *this;
    return j;
  }
};
