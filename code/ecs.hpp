#ifndef _ECS_H__
#define _ECS_H__

#include <typeindex>
#include <unordered_map>
#include <variant>
#include <ranges>

#include "component_registry_types.hpp"
#include "serialization.hpp"
#include "../external/json.hpp"

namespace System {
  struct Physics {};
  struct Input {};
  struct Draw {};
  struct Animation {};
  struct Tile {};
  struct Player_Animation{};
  struct Player_Movement {};
  struct InGameMenu {};
  struct AI {};
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

  void serialize()
  {
    auto component_registries_values = std::views::values(_component_registries);
    std::vector<Component_Registry_Types> registries = {component_registries_values.begin(), component_registries_values.end()};
    nlohmann::json output;
    
    for (auto &&[id, to_purge] : _entities) {
      if (to_purge) {
        continue;
      }
      for (auto &registry : registries) {
        do_serialize(id, output, registry);
      }
    }
    save(output);
  }

private:
  std::unordered_map<unsigned long long, bool> _entities;
  std::unordered_map<std::type_index, Component_Registry_Types> _component_registries;
  unsigned long long _latest{0};
};

#endif
