#include "ecs.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

#include "../../external/json.hpp"
#include "../AI/npc.hpp"
#include "../components/all_components.hpp"
#include "../management/resources.hpp"
#include "../management/serialization.hpp"
#include "../management/settings.hpp"
#include "../systems/available_systems.hpp"
#include "../utils/movement.hpp"
#include "../utils/time.hpp"

#include <iostream>

/**********************************************
 * Forward declaration of Utilitary functions *
 **********************************************/
static void step_kinematics(Kinematics &kinematics);
static void push_if_keydown(KeyboardKey key, std::vector<KeyboardKey> &keys);
static void update_keys_down(std::vector<KeyboardKey> &keys);

/**********************************************
 * Implementation of systems                  *
 **********************************************/

template <> void ECS::run_system<System::Tile>()
{
  for (auto &id : all_components<View, Kinematics>()) {
    View &view = component<View>(id);
    if (!view.active) {
      continue;
    }
    view.camera.target = component<Kinematics>(id).position;
    BeginMode2D(view.camera);
    for (const auto &tile_id : all_components<Tile>()) {
      Tile &tile = component<Tile>(tile_id);
      tile.tex->draw(tile.src_rect, component<Kinematics>(tile.entity_id).position, view.tint, tile.rotation, tile.flip);
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Animation>()
{
  for (const auto &id : all_components<View, Collider>()) {
    View &view = component<View>(id);
    if (!view.active) {
      continue;
    }
    Collider &curr_body = component<Collider>(id);
    view.camera.target = {curr_body.bound.x, curr_body.bound.y};
    BeginMode2D(view.camera);
    for (const auto &anim_id : all_components<Anim>()) {
      auto &curr_anim = component<Anim>(anim_id);
      Collider &anim_pos = component<Collider>(anim_id);
      Vector2 position = {anim_pos.bound.x, anim_pos.bound.y};
      float dist_x = std::abs(view.camera.target.x - position.x);
      if (dist_x < Settings::SCREEN_WIDTH / 4) {
        curr_anim.frame.step();
        Resources::get_resource_manager().texture(curr_anim.texture_id).draw(curr_anim.frame.src_rect, position, view.tint, 0.f, curr_anim.flip);
      }
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Player_Animation>()
{
  for (const auto &player_id : all_components<Player, Anim, Input, Kinematics>()) {
    auto &curr_anim = component<Anim>(player_id);
    auto &kinematics = component<Kinematics>(player_id);
    curr_anim.flip.horizontally = kinematics.velocity.x < 0.f ? true : kinematics.velocity.x > 0.f ? false : curr_anim.flip.horizontally;
    auto &keys_pressed = component<Input>(player_id).keys_pressed;

    if (keys_pressed.empty()) {
      auto &player_controls = component<Controls>(player_id);
      std::string idle_anim = kinematics.orientation.x > 0.f   ? "idle_right"
                               : kinematics.orientation.x < 0.f ? "idle_left"
                               : kinematics.orientation.y > 0.f ? "idle_down"
                               :"idle_up";
      auto &new_anim_settings = Resources::get_resource_manager().animation(curr_anim.name, idle_anim);
      // check if anim has changed
      if (new_anim_settings.start_pos.x != curr_anim.frame.start_pos.x || new_anim_settings.start_pos.y != curr_anim.frame.start_pos.y) {
        // update animation
        curr_anim.frame = Resources::get_resource_manager().animation(curr_anim.name, idle_anim);
      }
    }

    for (KeyboardKey &key : keys_pressed) {
      auto &player_controls = component<Controls>(player_id);
      std::string anim_id = player_controls.key_to_movement[key].action_id;
      auto &new_anim_settings = Resources::get_resource_manager().animation(curr_anim.name, anim_id);
      // check if anim has changed
      if (new_anim_settings.start_pos.x != curr_anim.frame.start_pos.x || new_anim_settings.start_pos.y != curr_anim.frame.start_pos.y) {
        // update animation
        curr_anim.frame = Resources::get_resource_manager().animation(curr_anim.name, anim_id);
      }
    }
  }
}

template <> void ECS::run_system<System::Draw>()
{
  BeginDrawing();
  ClearBackground(BLACK);
  run_system<System::Tile>();
  run_system<System::Player_Animation>();
  run_system<System::Animation>();
  EndDrawing();
}

template <> void ECS::run_system<System::Player_Movement>()
{
  for (const auto &id : all_components<Player, Kinematics, Collider, Input, Controls>()) {
    auto &player_body = component<Collider>(id);
    auto &kinematics = component<Kinematics>(id);
    kinematics.velocity.x = 0.f;
    kinematics.velocity.y = 0.f;
    for (const KeyboardKey &key : component<Input>(id).keys_pressed) {
      component<Controls>(id).key_to_movement.at(key).action(kinematics, player_body.collision_sides);
    }
    step_kinematics(kinematics);
    player_body.bound.x = kinematics.position.x;
    player_body.bound.y = kinematics.position.y;
  }
}

template <> void ECS::run_system<System::Physics>()
{
  for (const auto &id : all_components<Collider, Kinematics>()) {
    // register collisions
    auto &body = component<Collider>(id);
    body.collision_sides.clear();
    for (const auto &other_id : all_components<Collider>()) {
      if (other_id == id) {
        continue;
      }
      auto &other = component<Collider>(other_id);
      Side collision_side = get_collision_side(body.bound, other.bound);
      if (collision_side != Side::NONE) {
        body.collision_sides.insert(collision_side);
      }
    }
  }
}

// register input
template <> void ECS::run_system<System::Input>()
{
  for (const auto &id : all_components<Input>()) {
    auto &input = component<Input>(id);
    if (input.active) {
      update_keys_down(input.keys_pressed);
    }
  }
}

template <> void ECS::run_system<System::AI>()
{
  for (const auto &id : all_components<AI>()) {
    //component<Input>(id).keys_pressed = NPC::keys_down(component<AI>(id));
  }
}

template <> void ECS::run_system<System::InGameMenu>()
{
  serialize();
  ECS subsystem;
  for (const auto &id : all_components<Player, Input>()) {
    const auto &keys_pressed = component<Input>(id).keys_pressed;
    if (std::find(keys_pressed.begin(), keys_pressed.end(), KEY_ENTER) == keys_pressed.end()) {
      continue;
    }
    unsigned long long menu_id = subsystem.spawn_entity(Input(true));
    // it's flickering
    while (!WindowShouldClose()) {
      cap_framerate();
      BeginDrawing();
      auto &menu_keys_pressed = subsystem.component<Input>(menu_id).keys_pressed;
      if (std::find(menu_keys_pressed.begin(), menu_keys_pressed.end(), KEY_ENTER) != menu_keys_pressed.end()) {
        break;
      }
      EndDrawing();
      // it needs to be called after drawing, else it will return the same value every time
      // you can improve it after adding an image for the menu
      subsystem.run_system<System::Input>();
    }
  }
}

void ECS::serialize()
{
  nlohmann::json output;

  for (int i = 0; i < _latest; ++i) {
    for (auto &&[type, registry] : _component_registries) {
      serialize_component(i, output, registry.get());
    }
  }
  save(output);
}

/*****************************************
 * Implementation of Utilitary functions *
 *****************************************/

static void step_kinematics(Kinematics &kinematics)
{
  const float dt = GetFrameTime();
  kinematics.position.x += kinematics.velocity.x * dt + (kinematics.acceleration.x * dt * dt) / 2;
  kinematics.velocity.x += kinematics.acceleration.x * dt;
  kinematics.position.y += kinematics.velocity.y * dt;//  +(kinematics.acceleration.y * dt * dt) / 2;
}

static void push_if_keydown(KeyboardKey key, std::vector<KeyboardKey> &keys)
{
  if (IsKeyDown(key)) {
    keys.push_back(key);
  }
}

static void update_keys_down(std::vector<KeyboardKey> &keys)
{
  keys.clear();
  push_if_keydown(KEY_LEFT, keys);
  push_if_keydown(KEY_RIGHT, keys);
  push_if_keydown(KEY_UP, keys);
  push_if_keydown(KEY_DOWN, keys);
  push_if_keydown(KEY_ENTER, keys);
  push_if_keydown(KEY_A, keys);
  push_if_keydown(KEY_W, keys);
  push_if_keydown(KEY_D, keys);
  push_if_keydown(KEY_S, keys);
}