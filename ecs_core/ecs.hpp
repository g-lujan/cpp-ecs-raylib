#ifndef _ECS_H__
#define _ECS_H__

#include "components.hpp"
#include "component_registry.hpp"
#include <memory>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

template <typename... Type> constexpr std::unordered_set<std::type_index> FILTER() { return {typeid(Type)...}; }

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
    // https://stackoverflow.com/questions/68872572/lambda-call-operator-and-parenthesized-lambda
    (
        [&] {
          components.entity_id = entity_id;
          std::type_index component_type = typeid(components);
          if (_component_registries.find(component_type) == _component_registries.end()) {
            _component_registries.emplace(std::make_pair(component_type, std::make_unique<Component_Registry<decltype(components)>>()));
          }
          static_cast<Component_Registry<decltype(components)> *>(_component_registries[component_type].get())->record(entity_id, components);
        }(),
        ...);
    return entity_id;
  }

  template <typename T> Component_Registry<T> *component_registry()
  {
    return static_cast<Component_Registry<T> *>(_component_registries[typeid(T)].get());
  }

  template <typename... Component_Types> std::vector<unsigned long long> all_components()
  {
    auto types = FILTER<Component_Types...>();
    std::unordered_map<unsigned long long, int> id_freq;
    std::vector<unsigned long long> filtered_ids;
    for (std::type_index type : types) {
      for (auto id : _component_registries[type]->all_ids()) {
        id_freq[id]++;
        if (id_freq[id] == types.size()) {
          filtered_ids.push_back(id);
        }
      }
    }
    return filtered_ids;
  }

  template <typename T> T &component(const unsigned long long entity_id) { return component_registry<T>()->get(entity_id); }

  template <typename T> void run_system();

  void serialize();

private:
  std::unordered_map<unsigned long long, bool> _entities;
  std::unordered_map<std::type_index, std::unique_ptr<Component_Registry_Base>> _component_registries;
  unsigned long long _latest{0};
};

#endif
