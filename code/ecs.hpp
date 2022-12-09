#ifndef _ECS_H__
#define _ECS_H__

#include <typeindex>
#include <unordered_map>
#include <variant>

#include "components.hpp"

// TODO find solution where you don't need to keep track of all types of Components in the variant
using Component_Registry_Types = std::variant<
    Component_Registry<Player>,
    Component_Registry<Health>,
    Component_Registry<Position>,
    Component_Registry<Collider>,
    Component_Registry<Anim>,
    Component_Registry<View>,
    Component_Registry<Input>,
    Component_Registry<Tile>>;

namespace System {
  struct Collision {};
  struct Kinematics {};
  struct Input {};
  struct Draw {};
  struct Animation {};
  struct Tile {};
  struct Player_Animation{};
  struct Player_Movement {};
} // namespace System

class ECS {
public:
  ECS() {}

  unsigned long long create()
  {
    _latest++;
    _entities.insert({_latest, false});
    return _latest;
  }

  void mark_purge(const unsigned long long entity_id) { _entities.insert({entity_id, true}); }

  void purge()
  {
    for (const auto &[entity_id, value] : _entities) {
      if (_entities.find(entity_id) != _entities.end()) {
        _entities.erase(_entities.find(entity_id));
      }
    }
  }

  template <typename... ComponentTypes> unsigned long long spawn_entity(ComponentTypes... components)
  {
    unsigned long long entity_id = create();
    (
        [&] {
          components.entity_id = entity_id;
          std::type_index component_type = typeid(components);
          if (_component_registries.find(component_type) == _component_registries.end()) {
            _component_registries[component_type] = Component_Registry<decltype(components)>();
          }
          std::get<Component_Registry<decltype(components)>>(_component_registries[component_type]).record(entity_id, components);
        }(),
        ...);
    return entity_id;
  }

  template <typename T> T &component(const unsigned long long entity_id)
  {
    return std::get<Component_Registry<T>>(_component_registries[typeid(T)]).get(entity_id);
  }

  template <typename T> Component_Registry<T> &component_registry() { return std::get<Component_Registry<T>>(_component_registries[typeid(T)]); }

  template <typename T> void run_system();

private:
  std::unordered_map<unsigned long long, bool> _entities;
  std::unordered_map<std::type_index, Component_Registry_Types> _component_registries;
  unsigned long long _latest{0};
};

#endif
