#include "tilemap.hpp"
#include "../../external/json.hpp"
#include "../components/all_components.hpp"
#include "../ecs_core/ecs.hpp"
#include "../management/resources.hpp"
#include "../management/settings.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

namespace Tilemap {
  using json = nlohmann::json;

  // Bits on the far end of the 32-bit gid are used for tile flags
  // Reference: https://doc.mapeditor.org/en/stable/reference/global-tile-ids/#tile-flipping
  constexpr unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;  // bit 32
  constexpr unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;    // bit 31
  constexpr unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;    // bit 30
  constexpr unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000; // bit 29, unused in our case

  /**********************************************
   * FORWARD DECLARATION OF UTILITARY FUNCTIONS *
   **********************************************/

  static std::optional<json> load_and_parse_json(const std::string &path);
  static bool is_tilemap_valid(std::optional<json> &tilemap_json);
  static bool is_tileset_valid(std::optional<json> &tileset_json);
  static float tile_rotation(const int raw_tile_id);
  static Graphics::Flip tile_flip(const int raw_tile_id);

  // Spawn functions
  static void spawn_actors(Tilemap::json &layer, ECS &ecs, Resources::Manager &resources_manager);
  static void spawn_collision_entities(json &layer, ECS &ecs);
  static void spawn_tiles(const int num_tileset_cols, const int map_width_in_tiles, Tilemap::json &layer, ECS &ecs, Graphics::Texture *texture);

  /**********************************************
   * IMPL OF PUBLIC FUNCTIONS                    *
   ***********************************************/

  bool load(ECS &ecs, const std::string &map_name)
  {
    Resources::Manager &resources_manager = Resources::get_resource_manager();
    std::optional<json> tilemap_json = load_and_parse_json(resources_manager.tilemap_path(map_name));
    if (!is_tilemap_valid(tilemap_json)) {
      return false;
    }
    // retrieve tileset path from the tilemap
    std::string tileset_path = Resources::Paths::BASE_PATH;
    for (json &tileset : tilemap_json.value()["tilesets"]) {
      if (tileset.contains("source")) {
        tileset_path += tileset["source"].get<std::string>();
      }
    }
    std::optional<json> tileset_json = load_and_parse_json(tileset_path);
    if (!is_tileset_valid(tileset_json)) {
      return false;
    }
    // iterate over tile idxs, spawning entities
    for (json &layer : tilemap_json.value()["layers"]) {
      std::string layer_name = layer["name"].get<std::string>();
      if (layer_name == "Collision") {
        spawn_collision_entities(layer, ecs);
      }
      if (layer_name == "Tiles") {
        const int num_tileset_cols = tileset_json.value()["imagewidth"].get<int>() / tileset_json.value()["tilewidth"].get<int>();
        const int map_width_in_tiles = tilemap_json.value()["width"].get<int>();
        spawn_tiles(num_tileset_cols, map_width_in_tiles, layer, ecs, &resources_manager.texture(map_name));
      }
      if (layer_name == "Spawn") {
        spawn_actors(layer, ecs, resources_manager);
      }
    }
    return true;
  }

  /*******************************
   * IMPL OF UTILITARY FUNCTIONS *
   *******************************/

  static void spawn_actors(Tilemap::json &layer, ECS &ecs, Resources::Manager &resources_manager)
  {
    for (json &object : layer["objects"]) {
      Vector2 spawn_point = {object["x"].get<float>(), object["y"].get<float>()};
      if (object["name"].get<std::string>() == "Player") {
        ecs.spawn_entity(Kinematics(spawn_point),
                         Collider({spawn_point.x, spawn_point.y, Settings::TILE_SIZE, Settings::TILE_SIZE}, Body_Type::Player, true),
                         Health(100, 100),
                         Anim("player", "player", resources_manager.animation("player", "idle")),
                         View({{Settings::SCREEN_WIDTH / 2, Settings::SCREEN_HEIGHT / 2}, spawn_point, 0.f, Settings::DEFAULT_ZOOM},
                              {255, 255, 255, 255},
                              true,
                              "hometown"),
                         Input(true),
                         Controls(),
                         Player());
      }
      if (object["name"].get<std::string>() == "Enemy1") {
        ecs.spawn_entity(Kinematics(spawn_point),
                         Collider({spawn_point.x, spawn_point.y, Settings::TILE_SIZE, Settings::TILE_SIZE}, Body_Type::Player, true),
                         Health(100, 100),
                         Anim("player", "player", resources_manager.animation("player", "idle")),
                         Input(true),
                         AI(true),
                         Controls(),
                         Player());
      }
    }
  }

  static void spawn_tiles(const int num_tileset_cols, const int map_width_in_tiles, Tilemap::json &layer, ECS &ecs, Graphics::Texture *texture)
  {
    for (int tile_x = 0, tile_y = 0, tile_count = 0; tile_count < layer["data"].size(); ++tile_x, ++tile_count) {
      if (tile_x == map_width_in_tiles) {
        tile_y++;
        tile_x = 0;
      }
      const int raw_tile_id = layer["data"][tile_count].get<int>();
      if (raw_tile_id == 0) {
        continue;
      }
      const int tile_id =
          (raw_tile_id & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG | ROTATED_HEXAGONAL_120_FLAG));
      int src_x = 32 * ((tile_id - 1) % num_tileset_cols);
      int src_y = 32 * (int)((tile_id - 1) / num_tileset_cols);
      float rotation = tile_rotation(raw_tile_id);
      Graphics::Flip flip = tile_flip(raw_tile_id);
      ecs.spawn_entity(Tile(texture, Rectangle{static_cast<float>(src_x), static_cast<float>(src_y), 32, 32}, flip, rotation),
                       Kinematics(Vector2(static_cast<float>(tile_x * 32), static_cast<float>(tile_y * 32))));
    }
  }

  static void spawn_collision_entities(json &layer, ECS &ecs)
  {
    for (json &object : layer["objects"]) {
      Rectangle collider_rect = {object["x"].get<float>(), object["y"].get<float>(), object["width"].get<float>(), object["height"].get<float>()};
      ecs.spawn_entity(Collider(collider_rect, Body_Type::Wall, false));
    }
  }

  /************************************
   * UTILITY FUNCTIONS IMPLEMENTATIONS *
   *************************************/

  static std::optional<json> load_and_parse_json(const std::string &path)
  {
    std::ifstream loaded_json_stream(path);
    if (!loaded_json_stream.is_open()) {
#ifdef _WIN32 
        // https://developercommunity.visualstudio.com/t/strerrorlen_s-function-not-implemented/780241?space=62&q=__builtin_function
        std::cerr << "Failed to open file: " << path << ". Error: " << strerror(errno) << std::endl;
#else
      size_t errmsglen = strerrorlen_s(errno) + 1;
      char errmsg[errmsglen];
      strerror_s(errmsg, errmsglen, errno);
      std::cerr << "Failed to open file: " << path << ". Error: " << errmsg << std::endl;
#endif
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

  static bool is_tilemap_valid(std::optional<json> &tilemap_json) { return tilemap_json.has_value() && tilemap_json.value().contains("width"); }

  static bool is_tileset_valid(std::optional<json> &tileset_json)
  {
    return tileset_json.has_value() && tileset_json.value().contains("imagewidth") && tileset_json.value().contains("tilewidth") &&
           tileset_json.value()["tilewidth"].get<int>() > 0;
  }

  static float tile_rotation(const int raw_tile_id)
  {
    bool horizontal_flip = (raw_tile_id & FLIPPED_HORIZONTALLY_FLAG);
    bool vertical_flip = (raw_tile_id & FLIPPED_VERTICALLY_FLAG);
    bool anti_diag_flip = (raw_tile_id & FLIPPED_DIAGONALLY_FLAG);
    return anti_diag_flip && horizontal_flip ? 90.f : anti_diag_flip && vertical_flip ? -90.f : 0.f;
  }

  static Graphics::Flip tile_flip(const int raw_tile_id)
  {
    return Graphics::Flip(raw_tile_id & FLIPPED_HORIZONTALLY_FLAG, raw_tile_id & FLIPPED_VERTICALLY_FLAG);
  }

} // namespace Tilemap
