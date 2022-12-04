#ifndef _COMPONENTS_H__
#define _COMPONENTS_H__

#include "raylib.h"
#include "resources.hpp"

#include <algorithm>
#include <cfloat>
#include <functional>
#include <ranges>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <stack>

struct Component {
  unsigned long long entity_id;
};

template <typename T> class Component_Registry {
  static_assert(std::is_base_of_v<Component, T>, "Component Registry should be based on a Component type");

public:
  void record(const unsigned long long id, const T &component) { _components.insert({id, component}); }

  // TODO improve safety of this call
  T &get(const unsigned long long id) { return _components.at(id); }
  bool has(const unsigned long long id) { return _components.find(id) != _components.end(); }

  std::vector<unsigned long long> all_ids()
  {
    auto keys = std::views::keys(_components);
    return {keys.begin(), keys.end()};
  }

  std::vector<unsigned long long> all_ids_ordered()
  {
    auto ids = all_ids();
    std::sort(ids.begin(), ids.end());
    return ids;
  }

  void remove(const unsigned long long id) { _components.remove(id); }

  void purge(const std::vector<unsigned long long> ids)
  {
    for (const auto &id : ids) {
      _components.remove(id);
    }
  }

  std::string id() { return _id; }

private:
  std::unordered_map<unsigned long long, T> _components;
  std::string _id{typeid(T).name()};
};

enum class Body_Type {
  Player,
  Enemy,
  Door,
  Wall,
  Bullet,
  Sprite, // to delete when position and body decoupled
};

struct Position : public Component {
  Position(float x, float y) : x{x}, y{y} {}
  float x;
  float y;
};

struct Velocity : public Component {
  float x;
  float y;
};

// TODO: rect x and y should be relative to position
struct Collider : public Component {
  Collider(const Rectangle rect, const Body_Type type) : bound{rect}, type{type}, rot{0.f}, colliding{false} {}
  Rectangle bound;
  Body_Type type;
  float rot;
  bool colliding;
  bool grounded{false};
};

struct Health : public Component {
  Health(int curr, int max) : curr{curr}, max{max} {}
  int curr;
  int max;
};

struct Player : public Component {};

struct Anim : public Component {
  Anim(const std::string name, Graphics::Texture *tex, Graphics::Animation_Settings &settings) : name{name}, tex{tex}, settings{settings} {}
  Graphics::Texture *tex{nullptr};
  Graphics::Animation_Settings settings;
  std::string name;
};

struct View : public Component {
  View(Camera2D cam, Color tint, bool active, std::string map) : camera{cam}, tint{tint}, active{active}, map{map} {}
  Camera2D camera{0};
  Color tint;
  std::string map;
  bool active{false};
};

struct Tile : public Component {
  Tile(Graphics::Texture *tex, Rectangle src_rect) : tex{tex}, src_rect{src_rect} {}
  Graphics::Texture *tex{nullptr};
  Rectangle src_rect;
};

struct Input : public Component {
  Input(bool active) : active{active} {}
  bool active;
  std::vector<KeyboardKey> keys_pressed;
  bool changed{true};
};

#endif
