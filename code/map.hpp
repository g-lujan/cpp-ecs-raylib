#ifndef _MAP_H__
#define _MAP_H__

#include <fstream>
#include "../code/resources.hpp"
#include "../external/json.hpp"
#include "../code/ecs.hpp"

using json = nlohmann::json;

class Map {
public:
  Map(const Resources::Map map);

  int width() { return _tilemap_json["width"].get<int>();  }

  int tileset_cols() { return _tileset_json["imagewidth"].get<int>() / _tileset_json["tilewidth"].get<int>(); }

  bool active() { return _active; }

  void generate_components(ECS &ecs);


private:
  json _tilemap_json;
  json _tileset_json;
  bool _active{false};
};

#endif // !_MAP_H__
