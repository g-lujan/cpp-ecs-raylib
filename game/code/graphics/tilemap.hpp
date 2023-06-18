#ifndef _TILEMAP_H__
#define _TILEMAP_H__

#include <string>

class ECS;

namespace Tilemap {
  bool load(ECS &ecs, const std::string &map_name);
} // namespace Tilemap

#endif // !_MAP_H__
