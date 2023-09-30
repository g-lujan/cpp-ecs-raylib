#include "serialization.hpp"
#include <fstream>
#include <iomanip>
#include <raylib.h>

#include "../components/all_components.hpp"
#include "../ecs_core/components.hpp"

using json = nlohmann::json;

void serialize_component(const unsigned long long id, nlohmann::json &output, Component_Registry_Base *registry)
{
  if (registry->has(id) && registry->serializable(id)) {
    output[std::to_string(id)][registry->type_name()] = registry->serialize(id)->data;
  }
}

void save(nlohmann::json &output)
{
  std::ofstream o("save_file.json");
  o << std::setw(4) << output << std::endl;
}

/***********************
 * Components <=> json *
 ***********************/

void to_json(nlohmann::json &j, const Vector2 &vec2) { j = json{{"x", vec2.x}, {"y", vec2.y}}; }

void from_json(const nlohmann::json &j, Vector2 &vec2)
{
  j.at("x").get_to(vec2.x);
  j.at("y").get_to(vec2.y);
}

void to_json(nlohmann::json &j, const Rectangle &rec) { j = json{{"x", rec.x}, {"y", rec.y}, {"height", rec.height}, {"width", rec.width}}; }

void from_json(const nlohmann::json &j, Rectangle &rec)
{
  j.at("x").get_to(rec.x);
  j.at("y").get_to(rec.y);
  j.at("height").get_to(rec.height);
  j.at("width").get_to(rec.width);
}

void to_json(json &j, const Health &health) { j = json{{"curr", health.curr}, {"max", health.max}}; }

void from_json(const json &j, Health &health)
{
  j.at("curr").get_to(health.curr);
  j.at("max").get_to(health.max);
}

void to_json(nlohmann::json &j, const Player &player) { j = json{{"player", true}}; }

void from_json(const nlohmann::json &j, Player &player) { j.at("player_id").get_to(player.entity_id); }

void to_json(nlohmann::json &j, const Kinematics &kinematics)
{
  j = json{{"position", kinematics.position}, {"velocity", kinematics.velocity}, {"acceleration", kinematics.acceleration}};
}

void from_json(const nlohmann::json &j, Kinematics &kinematics)
{
  j.at("position").get_to(kinematics.position);
  j.at("velocity").get_to(kinematics.velocity);
  j.at("acceleration").get_to(kinematics.acceleration);
}

void to_json(nlohmann::json &j, const Collider &collider)
{
  j = json{{"bound", collider.bound},
           {"type", collider.type},
           {"collision_sides", collider.collision_sides},
           {"rot", collider.rot},
           {"kinematic", collider.kinematic}};
}
void from_json(const nlohmann::json &j, Collider &collider)
{
  j.at("bound").get_to(collider.bound);
  j.at("type").get_to(collider.type);
  j.at("collision_sides").get_to(collider.collision_sides);
  j.at("rot").get_to(collider.rot);
  j.at("kinematic").get_to(collider.kinematic);
}

void to_json(nlohmann::json &j, const Anim &anim) { return; }
void from_json(const nlohmann::json &j, Anim &anim) { return; }

void to_json(nlohmann::json &j, const View &view) { return; }
void from_json(const nlohmann::json &j, View &view) { return; }

void to_json(nlohmann::json &j, const Input &input) { return; }
void from_json(const nlohmann::json &j, Input &input) { return; }

void to_json(nlohmann::json &j, const Tile &tile) { return; }
void from_json(const nlohmann::json &j, Tile &tile) { return; }

void to_json(nlohmann::json &j, const AI &ai) { return; }
void from_json(const nlohmann::json &j, AI &ai) { return; }

void to_json(nlohmann::json &j, const Controls &ai) { return; }
void from_json(const nlohmann::json &j, Controls &ai) { return; }