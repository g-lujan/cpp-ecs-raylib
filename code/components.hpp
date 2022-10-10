#ifndef _COMPONENTS_H__
#define _COMPONENTS_H__

#include "raylib.h"
#include "resources.hpp"

#include <cfloat>
#include <functional>
#include <ranges>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

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
};

enum class Movement_Type {
  IDLE,
  WALK,
  JUMPING,
};

// change to collider
struct Body : public Component {
  Body(Rectangle rect, Body_Type type) : rect{rect}, type{type}, rot{0.f}, colliding{false}, closest_x{FLT_MAX}, closest_y{FLT_MAX} {}
  Rectangle rect; // change to bound
  Body_Type type;
  float rot;
  bool colliding;
  float closest_x;
  float closest_y;
};

struct Health : public Component {
  Health(int curr, int max) : curr{curr}, max{max} {}
  int curr;
  int max;
};

struct Anim : public Component {
  Anim(Resources::Animation_Type type) : type{type} { anim = Resources::animations[type][KEY_NULL]; }
  Resources::Animation_Type type;
  Resources::Animation anim;
};

struct View : public Component {
  View(Camera2D camera, Color tint, bool active) : camera{camera}, tint{tint}, active{active} {}
  Camera2D camera{0};
  Color tint;
  bool active{false};
};

struct Input : public Component {
  Input(bool active) : active{active} {}
  bool active;
  KeyboardKey key_pressed{KEY_NULL};
  bool changed;
};

// Not a component
struct Movement {
  Movement_Type type{Movement_Type::IDLE};
  bool flip{false};
};

#endif
