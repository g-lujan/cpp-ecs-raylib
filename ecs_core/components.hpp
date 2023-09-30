#ifndef _COMPONENTS_H__
#define _COMPONENTS_H__

#include <algorithm>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

// Serialization is client defined
struct Serializable;

struct Component {
  unsigned long long entity_id;
  bool serializable = false;
  virtual std::unique_ptr<Serializable> serialize() = 0;
  virtual std::string type_name() const = 0;
};

#endif
