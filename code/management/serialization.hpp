#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include "../../external/json.hpp"

template <typename T> void serialize_component(unsigned long long id, nlohmann::json &output, T *registry)
{
  if (registry->has(id) && registry->serializable(id)) {
    output[std::to_string(id)][registry->type_name()] = registry->serialize(id);
  }
}

void save(nlohmann::json &output);

struct Vector2;
void to_json(nlohmann::json &j, const Vector2 &vec2);
void from_json(const nlohmann::json &j, Vector2 &vec2);

struct Rectangle;
void to_json(nlohmann::json &j, const Rectangle &rec);
void from_json(const nlohmann::json &j, Rectangle &rec);

struct Health;
void to_json(nlohmann::json &j, const Health &health);
void from_json(const nlohmann::json &j, Health &health);

struct Player;
void to_json(nlohmann::json &j, const Player &player);
void from_json(const nlohmann::json &j, Player &player);

struct Kinematics;
void to_json(nlohmann::json &j, const Kinematics &kinematics);
void from_json(const nlohmann::json &j, Kinematics &kinematics);

struct Collider;
void to_json(nlohmann::json &j, const Collider &collider);
void from_json(const nlohmann::json &j, Collider &collider);

struct Anim;
void to_json(nlohmann::json &j, const Anim &anim);
void from_json(const nlohmann::json &j, Anim &anim);

struct View;
void to_json(nlohmann::json &j, const View &view);
void from_json(const nlohmann::json &j, View &view);

struct Input;
void to_json(nlohmann::json &j, const Input &input);
void from_json(const nlohmann::json &j, Input &input);

struct Tile;
void to_json(nlohmann::json &j, const Tile &tile);
void from_json(const nlohmann::json &j, Tile &tile);

struct AI;
void to_json(nlohmann::json &j, const AI &ai);
void from_json(const nlohmann::json &j, AI &ai);

#endif