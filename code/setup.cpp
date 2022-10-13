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
        Body({Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4, 16, 16}, Body_Type::Player),
        Health(100, 100),
        Anim(Resources::Animation_Type::PLAYER),
        View(
            {{Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, {Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4}, 0.f, 1.f},
            WHITE,
            true,
            Resources::Map::HOMETOWN),
        Input(true));

    ecs.spawn_entity(
        Body({3 * Settings::SCREEN_WIDTH / 4, 3 * Settings::SCREEN_HEIGHT / 4, 16, 16}, Body_Type::Player),
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

  void map(ECS &ecs)
  {
    // load map
    std::ifstream tilemap_input_stream("./resources/images/ville.json"); // add some erro handling
    json tilemap = json::parse(tilemap_input_stream);
    std::string src;
    for (auto &tileset : tilemap["tilesets"])
      src = "./resources/images/" + tileset["source"].get<std::string>();
    std::ifstream tileset_input_stream(src);
    json tileset = json::parse(tileset_input_stream);
    // number of columns in the tilesheet
    int ncols = tileset["imagewidth"].get<int>() / tileset["tilewidth"].get<int>();
    // load png
    Resources::textures.emplace("hometown", "resources/images/hometown.png");
    Resources::tiles[Resources::Map::HOMETOWN] = {};
    // iterate over tile idxs, spawning tile entities
    for (auto &layer : tilemap["layers"]) {
      int i = 0, j = 0, count = 0;
      for (auto &datum : layer["data"]) {
        int tile_idx = datum.get<int>();
        if (tile_idx == 0) {
          Resources::tiles[Resources::Map::HOMETOWN].push_back(Resources::EMPTY_TILE);
          continue;
        }
        if (i == tilemap["width"].get<int>())
          j++;
        i = i % tilemap["width"].get<int>();
        Resources::tiles[Resources::Map::HOMETOWN].push_back(
            Resources::Tile(&Resources::textures["hometown"], {32 * (tile_idx - 1) % ncols, 32 * (tile_idx - 1) / ncols}));
        ecs.spawn_entity(Body({i * 32, j * 32, 32, 32}, Body_Type::Sprite), Tile(count, Resources::Map::HOMETOWN));
        i++;
        count++;
      }
    }
  }
} // namespace Setup
