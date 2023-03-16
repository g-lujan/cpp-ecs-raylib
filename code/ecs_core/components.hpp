#ifndef _COMPONENTS_H__
#define _COMPONENTS_H__

#include <string>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>

// Serialization is client defined
struct Serializable;

struct Component {
  unsigned long long entity_id;
  bool serializable = false;
  virtual std::unique_ptr<Serializable> serialize() = 0;
  virtual std::string type_name() const = 0;
};

// Problema: a interface do registro não é capaz de comunicar a função fundamental de
// um registro, que é o T &get(const unsigned long long id)
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

  // TODO improve safety of this call
  T &get(const unsigned long long id) { return _components.at(id); }

  bool serializable(const unsigned long long id) { return get(id).serializable; }

  bool has(const unsigned long long id) { return _components.find(id) != _components.end(); }
  std::string type_name() const { return _id; }


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

  std::unique_ptr<Serializable> serialize(const unsigned long long id) { return get(id).serialize(); }

private:
  std::unordered_map<unsigned long long, T> _components;
  std::string _id{typeid(T).name()};
};

#endif
