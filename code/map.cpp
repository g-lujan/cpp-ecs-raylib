#include "map.hpp"

static json try_parse(std::ifstream &istream_json);
static bool is_valid_tileset(json &tileset);
static bool is_valid_tilemap(json &tilemap);

Map::Map(const Resources::Map map) {
  // try to load map istream from a json file
  std::ifstream tilemap_input_stream(Resources::map_json_path[map]);
  if (!tilemap_input_stream.is_open()) {
    std::cerr << "Failed to open file: " << Resources::map_json_path[map] << ". Error: " << strerror(errno) << std::endl;
    _active = false;
    return;
  }
  // parse map loaded as istream
  _tilemap_json = try_parse(tilemap_input_stream);
  if (_tilemap_json.is_null() || _tilemap_json.empty() || !is_valid_tilemap(_tilemap_json)) {
    _active = false;
    return;
  }
  // retrieve tileset path from the map
  std::string tileset_path = Resources::Images::BASE_PATH;
  for (auto &tileset : _tilemap_json["tilesets"]) {
    if (tileset.contains("source")) {
      tileset_path += tileset["source"].get<std::string>();
    }
  }

  if (tileset_path.empty()) {
    _active = false;
    return;
  }

  // load tileset from path
  std::ifstream tileset_input_stream(tileset_path);
  if (!tileset_input_stream.is_open()) {
    std::cerr << "Failed to open file: " << tileset_path << ". Error: " << strerror(errno) << std::endl;
    _active = false;
    return;
  }
  _tileset_json = try_parse(tileset_input_stream);
  if (_tileset_json.is_null() || _tileset_json.empty() || !is_valid_tileset(_tileset_json)) {
    _active = false;
    return;
  }
  _active = true;
}

void Map::generate_components(ECS &ecs)
{
  if (!_active) {
    std::cerr << "The map is not active, can't generate componets." << std::endl;
    return;
  }
  int num_tileset_cols = tileset_cols();
  // load png
  Resources::textures.emplace("hometown", Resources::Images::PATH_HOMETOWN);
  Resources::tiles[Resources::Map::HOMETOWN] = {};
  // iterate over tile idxs, spawning tile entities
  for (auto &layer : _tilemap_json["layers"]) {
    for (int tile_x = 0, tile_y = 0, tile_count = 0; tile_count < layer["data"].size(); ++tile_x, ++tile_count) {
      if (tile_x == width()) {
        tile_y++;
        tile_x = 0;
      }
      int tile_idx = layer["data"][tile_count].get<int>();
      if (tile_idx == 0) {
        Resources::tiles[Resources::Map::HOMETOWN].push_back(Resources::EMPTY_TILE);
      }
      else {
        int src_x = 32 * ((tile_idx - 1) % tileset_cols());
        int src_y = 32 * (int)((tile_idx - 1) / num_tileset_cols);
        Resources::tiles[Resources::Map::HOMETOWN].push_back(
            Resources::Tile(&Resources::textures["hometown"], {static_cast<float>(src_x), static_cast<float>(src_y)}));
      }
      ecs.spawn_entity(
          Body({static_cast<float>(tile_x * 32), static_cast<float>(tile_y * 32), 32, 32}, Body_Type::Sprite),
          Tile(tile_count, Resources::Map::HOMETOWN));
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