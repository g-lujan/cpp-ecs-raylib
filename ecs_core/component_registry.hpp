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

constexpr unsigned long long TOMBSTONE = 2e6;

class Component_Registry_Base {
public:
  virtual bool has(const unsigned long long guid) = 0;
  virtual bool serializable(const unsigned long long guid) = 0;
  virtual std::unique_ptr<Serializable> serialize(const unsigned long long guid) = 0;
  virtual std::string type_name() const = 0;
};

template <typename T> class Component_Registry : public Component_Registry_Base {
  static_assert(std::is_base_of_v<Component, T>, "Component Registry should be based on a Component type");

public:
  Component_Registry() : _next_free_idx{0L}, _sparse_guids(2000000, TOMBSTONE) {}

  void record(const unsigned long long global_id, const T &component)
  {
    if (_next_free_idx < ULLONG_MAX) {
      _components.push_back(component);
      _ids.push_back(global_id);
      _sparse_guids[global_id] = _next_free_idx++;
    }
  }

  inline T &get(const unsigned long long guid) { return _components[_sparse_guids[guid]]; }

  bool serializable(const unsigned long long guid) override { return get(guid).serializable; }

  bool has(const unsigned long long guid) { return _sparse_guids[guid] != TOMBSTONE; }

  std::string type_name() const { return _id; }

  std::vector<unsigned long long> &all_ids() { return _ids; }

  std::vector<T> &all_components() { return _components; }

  std::string id() { return _id; }

  std::unique_ptr<Serializable> serialize(const unsigned long long guid) { return get(guid).serialize(); }

private:
  std::vector<T> _components;
  std::vector<unsigned long long> _ids;
  std::vector<unsigned long long> _sparse_guids;

  unsigned long long _next_free_idx;
  std::string _id{typeid(T).name()};
};

#endif
