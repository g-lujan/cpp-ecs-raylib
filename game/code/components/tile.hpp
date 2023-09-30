#pragma once

#include "../graphics/graphics.hpp"
#include "components.hpp"

struct Tile : public Component {
  Tile(Graphics::Texture *tex, Rectangle src_rect, Graphics::Flip flip, float rotation = 0.f)
      : tex{tex}, src_rect{src_rect}, rotation{rotation}, flip{flip}
  {
  }

  virtual std::string type_name() const { return "Tile"; }

  virtual std::unique_ptr<Serializable> serialize()
  {
    nlohmann::json j = *this;
    return std::make_unique<Serializable>(j);
  }

  Graphics::Texture *tex{nullptr};
  Rectangle src_rect;
  float rotation;
  Graphics::Flip flip;
};
