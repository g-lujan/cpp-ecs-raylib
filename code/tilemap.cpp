#include "tilemap.hpp"
#include "../external/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace Tilemap {
  // Forward declaration of utilitary functions
  static json try_parse(std::ifstream &istream_json);
  static bool is_valid_tileset(json &tileset);
  static bool is_valid_tilemap(json &tilemap);

  bool load(ECS &ecs, Resources::Manager &resources_manager, const std::string &map_name)
  {
    // try to load map istream from a json file
    std::ifstream tilemap_input_stream(resources_manager.tilemap_path(map_name));
    if (!tilemap_input_stream.is_open()) {
      std::cerr << "Failed to open file: " << resources_manager.tilemap_path(map_name) << ". Error: " << strerror(errno) << std::endl;
      return false;
    }
    // parse map loaded as istream
    json tilemap_json = try_parse(tilemap_input_stream);
    if (tilemap_json.is_null() || tilemap_json.empty() || !is_valid_tilemap(tilemap_json)) {
      return false;
    }
    // retrieve tileset path from the map
    std::string tileset_path = Resources::Paths::BASE_PATH;
    for (auto &tileset : tilemap_json["tilesets"]) {
      if (tileset.contains("source")) {
        tileset_path += tileset["source"].get<std::string>();
      }
    }

    if (tileset_path.empty()) {
      return false;
    }

    // load tileset from path
    std::ifstream tileset_input_stream(tileset_path);
    if (!tileset_input_stream.is_open()) {
      std::cerr << "Failed to open file: " << tileset_path << ". Error: " << strerror(errno) << std::endl;
      return false;
    }
    json tileset_json = try_parse(tileset_input_stream);
    if (tileset_json.is_null() || tileset_json.empty() || !is_valid_tileset(tileset_json)) {
      return false;
    }

    int num_tileset_cols = tileset_json["imagewidth"].get<int>() / tileset_json["tilewidth"].get<int>();
    // load png
    // iterate over tile idxs, spawning tile entities
    for (auto &layer : tilemap_json["layers"]) {
      for (int tile_x = 0, tile_y = 0, tile_count = 0; tile_count < layer["data"].size(); ++tile_x, ++tile_count) {
        if (tile_x == tilemap_json["width"].get<int>()) {
          tile_y++;
          tile_x = 0;
        }
        int tile_idx = layer["data"][tile_count].get<int>();
        if (tile_idx == 0) {
          ecs.spawn_entity(
              Collider({static_cast<float>(tile_x * 32), static_cast<float>(tile_y * 32), 32, 32}, Body_Type::Sprite),
              Tile(&resources_manager.texture("empty"), Rectangle{0.f, 0.f, 0, 0})
          );
        }
        else {
          int src_x = 32 * ((tile_idx - 1) % num_tileset_cols);
          int src_y = 32 * (int)((tile_idx - 1) / num_tileset_cols);
          ecs.spawn_entity(
              Collider({static_cast<float>(tile_x * 32), static_cast<float>(tile_y * 32), 32, 32}, Body_Type::Sprite),
              Tile(&resources_manager.texture(map_name), Rectangle{static_cast<float>(src_x), static_cast<float>(src_y), 32, 32})
          );
        }
      }
    }
  }

  /*
   * Utilitary functions to parse the map
   */

  static json try_parse(std::ifstream &istream_json)
  {
    try {
      return json::parse(istream_json);
    }
    catch (const std::exception &e) {
      std::cerr << "Failed to parse json: " << istream_json.rdbuf() << ". Exception: " << e.what() << std::endl;
      return json::value_t::object;
    }
  }

  static bool is_valid_tileset(json &tileset)
  {
    if (!tileset.contains("imagewidth")) {
      std::cerr << "Invalid tileset json: it doesn't contain imagewidth field." << std::endl;
      return false;
    }
    if (!tileset.contains("tilewidth")) {
      std::cerr << "Invalid tileset json: it doesn't contain tilewidth field." << std::endl;
      return false;
    }
    return true;
  }

  static bool is_valid_tilemap(json &tilemap)
  {
    if (!tilemap.contains("tilesets")) {
      std::cerr << "Invalid tilemap json: it doesn't contain tilesets field." << std::endl;
      return false;
    }
    if (!tilemap.contains("layers")) {
      std::cerr << "Invalid tilemap json: it doesn't contain layers field." << std::endl;
      return false;
    }
    if (!tilemap.contains("width")) {
      std::cerr << "Invalid tilemap json: it doesn't contain width field." << std::endl;
      return false;
    }
    return true;
  }
} // namespace Tilemap