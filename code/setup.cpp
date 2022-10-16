#include "setup.hpp"
#include "../external/json.hpp"
#include "settings.hpp"
#include <fstream>
#include <string>
#include <vector>

namespace Setup {
  void players(ECS &ecs)
  {
    ecs.spawn_entity(
        Body({::floorf(Settings::SCREEN_WIDTH / 4), ::floorf(3 * Settings::SCREEN_HEIGHT / 4), 32, 32}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View(
            {{Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, {Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, 0.f, 1.f},
            WHITE,
            true,
            Resources::Map::HOMETOWN),
        Input(true));

    ecs.spawn_entity(
        Body({3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4, 32, 32}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View(
            {{3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             {3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4},
             0.f,
             1.f},
            {0, 0, 255, 255},
            true,
            Resources::Map::HOMETOWN),
        Input(false));
  }

  using json = nlohmann::json;

  json try_parse(std::ifstream &istream_json)
  {
    try {
      return json::parse(istream_json);
    }
    catch (const std::exception &e) {
      std::cerr << "Failed to parse json: " << istream_json.rdbuf() << ". Exception: " << e.what() << std::endl;
      return json::value_t::object;
    }
  }

  bool is_valid_tileset(json &tileset)
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

  bool is_valid_tilemap(json &tilemap)
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

  bool map(ECS &ecs, const Resources::Map map)
  {
    // load map
    std::ifstream tilemap_input_stream(Resources::map_json_path[map]);
    if (!tilemap_input_stream.is_open()) {
      std::cerr << "Failed to open file: " << Resources::map_json_path[map] << ". Error: " << strerror(errno) << std::endl;
      return false;
    }
    // try to load respective tileset
    json tilemap_json = try_parse(tilemap_input_stream);
    if (tilemap_json.empty() || !is_valid_tilemap(tilemap_json)) {
      return false;
    }
    std::string tileset_path = "";
    for (auto &tileset : tilemap_json["tilesets"]) {
      if (tileset.contains("source")) {
        tileset_path = Resources::Images::BASE_PATH + tileset["source"].get<std::string>();
      }
    }
    if (tileset_path.empty()) {
      return false;
    }
    std::ifstream tileset_input_stream(tileset_path);
    if (!tileset_input_stream.is_open()) {
      std::cerr << "Failed to open file: " << tileset_path << ". Error: " << strerror(errno) << std::endl;
      return false;
    }
    json tileset_json = try_parse(tileset_input_stream);
    if (tileset_json.empty() || !is_valid_tileset(tileset_json)) {
      return false;
    }

    int num_tileset_cols = tileset_json["imagewidth"].get<int>() / tileset_json["tilewidth"].get<int>();
    // load png
    Resources::textures.emplace("hometown", Resources::Images::PATH_HOMETOWN);
    Resources::tiles[Resources::Map::HOMETOWN] = {};
    // iterate over tile idxs, spawning tile entities
    for (auto &layer : tilemap_json["layers"]) {
      for (int tile_x = 0, tile_y = 0, tile_count = 0; tile_count < layer["data"].size(); ++tile_x, ++tile_count) {
        if (tile_x == tilemap_json["width"].get<int>()) {
          tile_y++;
          tile_x = 0;
        }
        int tile_idx = layer["data"][tile_count].get<int>();
        if (tile_idx == 0) {
          Resources::tiles[Resources::Map::HOMETOWN].push_back(Resources::EMPTY_TILE);
        }
        else {
          Resources::tiles[Resources::Map::HOMETOWN].push_back(Resources::Tile(&Resources::textures["hometown"],
              {32 * static_cast<float>((tile_idx - 1) % num_tileset_cols), static_cast<float>(32 * (int)((tile_idx - 1) / num_tileset_cols))}
          ));
        }
        ecs.spawn_entity(Body({static_cast<float>(tile_x * 32), static_cast<float>(tile_y * 32), 32, 32}, Body_Type::Sprite), Tile(tile_count, Resources::Map::HOMETOWN));
      }
    }
  }
} // namespace Setup
