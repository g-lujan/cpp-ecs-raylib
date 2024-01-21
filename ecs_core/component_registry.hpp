#ifndef _COMPONENT_REGISTRY_H__
#define _COMPONENT_REGISTRY_H__

#include "components.hpp"
#include <algorithm>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

class Component_Registry_Base {
public:
  virtual bool has(const unsigned long long id) = 0;
  virtual bool serializable(const unsigned long long id) = 0;
  virtual std::unique_ptr<Serializable> serialize(const unsigned long long id) = 0;
  virtual std::string type_name() const = 0;
};

template <typename T> class Component_Registry : public Component_Registry_Base {
  static_assert(std::is_base_of_v<Component, T>, "Component Registry should be based on a Component type");

public:
  void record(const unsigned long long id, const T &component) { _components.insert({id, component}); }

  T &get(const unsigned long long id) { return _components.at(id); }

  bool serializable(const unsigned long long id) { return get(id).serializable; }

  bool has(const unsigned long long id) { return _components.find(id) != _components.end(); }
  std::string type_name() const { return _id; }

  std::ranges::view auto all_ids() const { return std::views::keys(_components); }

  std::ranges::view auto all_components() const { return std::views::values(_components); }

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

  std::unique_ptr<Serializable> serialize(const unsigned long long id) { return get(id).serialize(); }

private:
  std::unordered_map<unsigned long long, T> _components;
  std::string _id{typeid(T).name()};
};

#endif
