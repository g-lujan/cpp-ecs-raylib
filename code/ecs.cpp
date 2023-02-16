#include "ecs.hpp"
#include "movement.hpp"
#include "raylib.h"
#include "raymath.h"
#include "resources.hpp"
#include "settings.hpp"
#include "npc.hpp"

template <> void ECS::run_system<System::Tile>()
{
  Component_Registry<Tile> &tile_registry = component_registry<Tile>();
  Component_Registry<Kinematics> &kinematics_registry = component_registry<Kinematics>();
  Component_Registry<View> &view_registry = component_registry<View>();

  for (const auto &view_id : view_registry.all_ids()) {
    auto &curr_view = view_registry.get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_pos = kinematics_registry.get(view_id).position;
    // update camera
    curr_view.camera.target = {curr_pos.x, curr_pos.y};
    BeginMode2D(curr_view.camera);
    // draw tiles
    for (const auto &tile_id : tile_registry.all_ids()) {
      Vector2 position = {kinematics_registry.get(tile_id).position.x, kinematics_registry.get(tile_id).position.y};
      auto &tile = tile_registry.get(tile_id);
      tile.tex->draw(tile.src_rect, position, curr_view.tint, tile.rotation, tile.flip);
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Animation>()
{
  Component_Registry<Anim> &anim_registry = component_registry<Anim>();
  Component_Registry<Collider> &body_registry = component_registry<Collider>();
  Component_Registry<View> &view_registry = component_registry<View>();
  Component_Registry<Input> &input_registry = component_registry<Input>();

  for (const auto &view_id : view_registry.all_ids()) {
    auto &curr_view = view_registry.get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_body = body_registry.get(view_id);
    curr_view.camera.target = {curr_body.bound.x, curr_body.bound.y};
    BeginMode2D(curr_view.camera);
    for (const auto &anim_id : anim_registry.all_ids_ordered()) {
      auto &curr_anim = anim_registry.get(anim_id);
      Vector2 position = {body_registry.get(anim_id).bound.x, body_registry.get(anim_id).bound.y};
      float dist_x = std::abs(curr_view.camera.target.x - position.x);
      if (dist_x < Settings::SCREEN_WIDTH / 4) {
        Graphics::step(curr_anim.settings);
        curr_anim.tex->draw(curr_anim.settings.curr_frame, position, curr_view.tint, 0.f, curr_anim.flip);
      }
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Player_Animation>()
{
  Component_Registry<Player> &player_registry = component_registry<Player>();
  Component_Registry<Anim> &anim_registry = component_registry<Anim>();
  Component_Registry<Input> &input_registry = component_registry<Input>();
  Component_Registry<Kinematics> &kinematics_registry = component_registry<Kinematics>();

  for (const auto &player_id : player_registry.all_ids()) {
    auto &curr_anim = anim_registry.get(player_id);
    auto &keys_pressed = input_registry.get(player_id).keys_pressed;
    auto &kinematics = kinematics_registry.get(player_id);
    curr_anim.flip.horizontally = kinematics.velocity.x < 0.f ? true : kinematics.velocity.x > 0.f ? false : curr_anim.flip.horizontally;

    if (kinematics.velocity.y != 0.f) {
      auto &jump_anim_settings = Resources::get_resource_manager().animation(curr_anim.name, KEY_SPACE);
      // check if anim has changed
      if (jump_anim_settings.start_frame_pos.x != curr_anim.settings.start_frame_pos.x ||
          jump_anim_settings.start_frame_pos.y != curr_anim.settings.start_frame_pos.y) {
        // update animation
        curr_anim.settings = jump_anim_settings;
      }
      return;
    }
    else if (keys_pressed.empty()) {
      curr_anim.settings = Resources::get_resource_manager().animation(curr_anim.name, KEY_NULL);
    }

    for (KeyboardKey &key : keys_pressed) {
      auto &new_anim_settings = Resources::get_resource_manager().animation(curr_anim.name, key);
      // check if anim has changed
      if (new_anim_settings.start_frame_pos.x != curr_anim.settings.start_frame_pos.x ||
          new_anim_settings.start_frame_pos.y != curr_anim.settings.start_frame_pos.y) {
        // update animation
        curr_anim.settings = Resources::get_resource_manager().animation(curr_anim.name, key);
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
  Component_Registry<Player> &player_registry = component_registry<Player>();
  Component_Registry<Kinematics> &kinematics_registry = component_registry<Kinematics>();
  Component_Registry<Collider> &body_registry = component_registry<Collider>();
  Component_Registry<Input> &input_registry = component_registry<Input>();

  for (const auto &id : player_registry.all_ids()) {
    auto &player_body = body_registry.get(id);
    auto &keys_pressed = input_registry.get(id).keys_pressed;
    auto &kinematics = kinematics_registry.get(id);
    kinematics.velocity.x = 0;
    for (const KeyboardKey &key : keys_pressed) {
      key_to_movement.at(key)(kinematics, player_body.collision_sides);
    }
    step_kinematics(kinematics);
    player_body.bound.x = kinematics.position.x;
    player_body.bound.y = kinematics.position.y;
  }
}

template <> void ECS::run_system<System::Physics>()
{
  Component_Registry<Collider> &body_registry = component_registry<Collider>();
  Component_Registry<Kinematics> &kinematics_registry = component_registry<Kinematics>();

  for (const auto &id : body_registry.all_ids()) {
    auto &body = body_registry.get(id);
    if (!kinematics_registry.has(id)) {
      continue;
    }
    // register collisions
    body.collision_sides.clear();
    for (const auto &other_id : body_registry.all_ids()) {
      if (other_id == id) {
        continue;
      }
      auto &other = body_registry.get(other_id);
      Side collision_side = get_collision_side(body.bound, other.bound);
      if (collision_side != Side::NONE) {
        body.collision_sides.insert(collision_side);
      }
    }
    // gravity
    auto &kinematics = kinematics_registry.get(id);
    if (!body.collision_sides.contains(Side::BOTTON)) {
      kinematics.acceleration.y = 40;
    }
    else {
      kinematics.acceleration.y = 0;
      kinematics.velocity.y = 0;
    }
  }
}

static std::vector<KeyboardKey> keys_down(void)
{
  std::vector<KeyboardKey> keys;
  if (IsKeyDown(KEY_LEFT)) {
    keys.push_back(KEY_LEFT);
  }
  if (IsKeyDown(KEY_RIGHT)) {
    keys.push_back(KEY_RIGHT);
  }
  if (IsKeyDown(KEY_SPACE)) {
    keys.push_back(KEY_SPACE);
  }
  return keys;
}

// register input
template <> void ECS::run_system<System::Input>()
{
  Component_Registry<Input> &input_registry = component_registry<Input>();
  Component_Registry<AI> &ai_registry = component_registry<AI>();

  for (const auto &id : input_registry.all_ids()) {
    if (!input_registry.get(id).active) {
      continue;
    }
    input_registry.get(id).keys_pressed = keys_down();
  }

  for (const auto &id : ai_registry.all_ids()) {
    input_registry.get(id).keys_pressed = NPC::keys_down(ai_registry.get(id));
  }
}
