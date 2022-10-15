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
        Body({std::floor(Settings::SCREEN_WIDTH / 4), std::floor(3 * Settings::SCREEN_HEIGHT / 4), 32, 32}, Body_Type::Player),
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
    // number of columns in the tileset
    int ncols = tileset_json["imagewidth"].get<int>() / tileset_json["tilewidth"].get<int>();
    // load png
    Resources::textures.emplace("hometown", Resources::Images::PATH_HOMETOWN);
    Resources::tiles[Resources::Map::HOMETOWN] = {};
    // iterate over tile idxs, spawning tile entities
    for (auto &layer : tilemap_json["layers"]) {
      int i = 0, j = 0, count = 0;
      for (auto &datum : layer["data"]) {
        int tile_idx = datum.get<int>();
        if (j == tilemap_json["width"].get<int>())
          i++;
        j = j % tilemap_json["width"].get<int>();
        if (tile_idx == 0) {
          Resources::tiles[Resources::Map::HOMETOWN].push_back(Resources::EMPTY_TILE);
        }
        else {
          Resources::tiles[Resources::Map::HOMETOWN].push_back(
              Resources::Tile(&Resources::textures["hometown"], {32 * ((tile_idx - 1) % ncols), 32 * (int)((tile_idx - 1) / ncols)}));
        }
        ecs.spawn_entity(Body({i * 32, j * 32, 32, 32}, Body_Type::Sprite), Tile(count, Resources::Map::HOMETOWN));
        j++;
        count++;
      }
    }
  }
} // namespace Setup
