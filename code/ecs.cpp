#include "ecs.hpp"
#include "movement.hpp"
#include "raylib.h"
#include "raymath.h"
#include "resources.hpp"
#include "settings.hpp"

template <> void ECS::run_system<System::Tile>()
{
  Component_Registry<Tile> &tile_registry = component_registry<Tile>();
  Component_Registry<Position> &pos_registry = component_registry<Position>();
  Component_Registry<View> &view_registry = component_registry<View>();

  for (const auto &view_id : view_registry.all_ids()) {
    auto &curr_view = view_registry.get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_pos = pos_registry.get(view_id);
    // update camera
    curr_view.camera.target = {curr_pos.x, curr_pos.y};
    BeginMode2D(curr_view.camera);
    // draw tiles
    for (const auto &tile_id : tile_registry.all_ids()) {
      Vector2 position = {pos_registry.get(tile_id).x, pos_registry.get(tile_id).y};
      float dist_x = std::abs(curr_view.camera.target.x - position.x);
      if (dist_x < Settings::SCREEN_WIDTH / 8) {
        auto &tile = tile_registry.get(tile_id);
        tile.tex->draw(tile.src_rect, position, curr_view.tint);
      }
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
        curr_anim.tex->draw(curr_anim.settings.curr_frame, position, curr_view.tint, curr_anim.settings.flip);
      }
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Player_Animation>() {
  Component_Registry<Player> &player_registry = component_registry<Player>();
  Component_Registry<Anim> &anim_registry = component_registry<Anim>();
  Component_Registry<Input> &input_registry = component_registry<Input>();
  Component_Registry<Collider> &collider_registry = component_registry<Collider>();

  for (const auto &player_id : player_registry.all_ids()) {
    auto &curr_anim = anim_registry.get(player_id);
    auto &keys_pressed = input_registry.get(player_id).keys_pressed;
    const bool flip = curr_anim.settings.flip;
    if (!collider_registry.get(player_id).grounded) {
      auto &jump_anim_settings = Resources::get_resource_manager().animation(curr_anim.name, KEY_SPACE);
      // check if anim has changed
      if (jump_anim_settings.start_frame_pos.x != curr_anim.settings.start_frame_pos.x ||
          jump_anim_settings.start_frame_pos.y != curr_anim.settings.start_frame_pos.y) {
        // update animation
        curr_anim.settings = jump_anim_settings;
      }
      curr_anim.settings.flip = flip;
      return;
    }
    else if (keys_pressed.empty()) {
        curr_anim.settings = Resources::get_resource_manager().animation(curr_anim.name, KEY_NULL);
        curr_anim.settings.flip = flip;
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

template <> void ECS::run_system<System::Physics>()
{
  Component_Registry<Collider> &body_registry = component_registry<Collider>();
  Component_Registry<Input> &input_registry = component_registry<Input>();
  Component_Registry<Position> &pos_registry = component_registry<Position>();

  for (const auto &id : input_registry.all_ids()) {
    auto &actor_body = body_registry.get(id);
    // process movement
    auto &keys_pressed = input_registry.get(id).keys_pressed;
    Collider collider_updated = actor_body;
    for (const KeyboardKey &key : keys_pressed) {
      auto &movement = key_to_movement.at(key);
      collider_updated.bound = move(collider_updated, movement);
    }
    for (const auto &body_id : body_registry.all_ids()) {
      if (body_id == id) {
        continue;
      }
      auto &other = body_registry.get(body_id);
      if (CheckCollisionRecs(collider_updated.bound, other.bound)) {
        if (actor_body.type == Body_Type::Player && other.type == Body_Type::Wall) {
            switch (get_collision_side(collider_updated.bound, other.bound)) {
            case Side::LEFT:
            case Side::RIGHT:
              collider_updated.bound.x = actor_body.bound.x;
              break;
            case Side::BOTTON:
              collider_updated.grounded = true;
              break;
            default:
              break;
            }
        }
      }
    }
    // apply gravity
    if (!collider_updated.grounded) {
      collider_updated.bound.y += Settings::STEP;
    }
    auto &curr_pos = pos_registry.get(id);
    curr_pos.x = collider_updated.bound.x;
    curr_pos.y = collider_updated.bound.y;
    actor_body = collider_updated;
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
  std::vector<KeyboardKey> curr_keys = keys_down();
  for (const auto &id : input_registry.all_ids()) {
    if (!input_registry.get(id).active) {
      continue;
    }
    input_registry.get(id).keys_pressed = curr_keys;
  }
}
