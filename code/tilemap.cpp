#include "tilemap.hpp"
#include "../external/json.hpp"
#include "resources.hpp"
#include <fstream>
#include <iostream>
#include <optional>

using json = nlohmann::json;

namespace Tilemap {
  // Forward declaration of utilitary functions
  static bool is_valid_tileset(json &tileset);
  static bool is_valid_tilemap(json &tilemap);

  static std::optional<json> load_and_parse_json(const std::string &path)
  {
    std::ifstream loaded_json_stream(path);
    if (!loaded_json_stream.is_open()) {
      std::cerr << "Failed to open file: " << path << ". Error: " << strerror(errno) << std::endl;
      return {};
    }
    try {
      return json::parse(loaded_json_stream);
    }
    catch (const std::exception &e) {
      std::cerr << "Failed to parse json: " << loaded_json_stream.rdbuf() << ". Exception: " << e.what() << std::endl;
      return {};
    }
  }

  bool load(ECS &ecs, const std::string &map_name)
  {
    Resources::Manager &resources_manager = Resources::get_resource_manager();
    std::optional<json> tilemap_json = load_and_parse_json(resources_manager.tilemap_path(map_name));
    if (!tilemap_json.has_value()) {
      return false;
    }
    // retrieve tileset path from the tilemap
    std::string tileset_path = Resources::Paths::BASE_PATH;
    for (auto &tileset : tilemap_json.value()["tilesets"]) {
      if (tileset.contains("source")) {
        tileset_path += tileset["source"].get<std::string>();
      }
    }
    std::optional<json> tileset_json = load_and_parse_json(tileset_path);
    if (!tileset_json.has_value()) {
      return false;
    }

    if (!tileset_json.has_value()) {
      return false;
    }
    if (!tileset_json.value().contains("imagewidth") || !tileset_json.value().contains("tilewidth")) {
      return false;
    }
    if (tileset_json.value()["tilewidth"].get<int>() == 0) {
      return false;
    }
    const int num_tileset_cols = tileset_json.value()["imagewidth"].get<int>() / tileset_json.value()["tilewidth"].get<int>();
    if (!tilemap_json.has_value() || !tilemap_json.value().contains("width")) {
      return false;
    }
    const int map_width_in_tiles = tilemap_json.value()["width"].get<int>();
    // iterate over tile idxs, spawning tile entities
    for (auto &layer : tilemap_json.value()["layers"]) {
      std::string curr = layer["name"].get<std::string>();
      if (layer["name"].get<std::string>() == "Collision") {
        for (auto &object : layer["objects"]) {
            ecs.spawn_entity(Collider({object["x"].get<float>(), object["y"].get<float>(), object["width"].get<float>(), object["height"].get<float>()}, Body_Type::Wall, false));
        }
      }
      if (layer["name"].get<std::string>() == "Tiles") {
        for (int tile_x = 0, tile_y = 0, tile_count = 0; tile_count < layer["data"].size(); ++tile_x, ++tile_count) {
          if (tile_x == map_width_in_tiles) {
            tile_y++;
            tile_x = 0;
          }
          int tile_idx = layer["data"][tile_count].get<int>();
          if (tile_idx == 0) {
            continue;
          }
        int src_x = 32 * ((tile_idx - 1) % num_tileset_cols);
        int src_y = 32 * (int)((tile_idx - 1) / num_tileset_cols);
        ecs.spawn_entity(
            Tile(&resources_manager.texture(map_name), Rectangle{static_cast<float>(src_x), static_cast<float>(src_y), 32, 32}),
            Position(static_cast<float>(tile_x * 32), static_cast<float>(tile_y * 32)));
        }
      }
    }
  }

  /*
   * Utilitary functions to parse the map
   */
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
