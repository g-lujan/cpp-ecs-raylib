#ifndef _ECS_H__
#define _ECS_H__

#include "component_registry.hpp"
#include "components.hpp"
#include <memory>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename... Type> constexpr std::unordered_set<std::type_index> FILTER() { return {typeid(Type)...}; }

class ECS {
public:
  ECS() : _latest{0} {}

  unsigned long long create()
  {
    _latest++;
    return _latest;
  }

  template <typename... ComponentTypes> unsigned long long spawn_entity(ComponentTypes... components)
  {
    unsigned long long entity_id = create();
    // https://stackoverflow.com/questions/68872572/lambda-call-operator-and-parenthesized-lambda
    (
        [&] {
          std::type_index component_type = typeid(components);
          if (_component_registries.find(component_type) == _component_registries.end()) {
            _component_registries.emplace(std::make_pair(component_type, std::make_unique<Component_Registry<decltype(components)>>()));
          }
          components.id = entity_id;
          static_cast<Component_Registry<decltype(components)> *>(_component_registries[component_type].get())->record(entity_id, components);

        }(),
        ...);
    return entity_id;
  }

  template <typename T> Component_Registry<T> *component_registry()
  {
    return static_cast<Component_Registry<T> *>(_component_registries[typeid(T)].get());
  }

  template <typename... ComponentTypes> std::vector<unsigned long long> all_components()
  {
    std::vector<unsigned long long> filtered_ids;
    (
        [&] {
          std::vector<unsigned long long> current_ids;
          std::type_index component_type = typeid(ComponentTypes);
          if (filtered_ids.empty()) {
            filtered_ids = static_cast<Component_Registry<ComponentTypes> *>(_component_registries[component_type].get())->all_ids();
          }
          else {
            Component_Registry<ComponentTypes> *registry = static_cast<Component_Registry<ComponentTypes> *>(_component_registries[component_type].get());
            for (unsigned long long id : filtered_ids) {
              if (registry->has(id)) {
                current_ids.push_back(id);
              }
            }
            filtered_ids = current_ids;
          }
        }(),
        ...);
    return filtered_ids;
  }

  template <typename T> T &component(const unsigned long long guid) { 
      return component_registry<T>()->get(guid); 
  }

  template <typename T> void run_system();

  void serialize();

private:
  std::unordered_map<std::type_index, std::unique_ptr<Component_Registry_Base>> _component_registries;
  unsigned long long _latest;
};

#endif
