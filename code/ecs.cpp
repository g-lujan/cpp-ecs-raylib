#include "ecs.hpp"
#include "raylib.h"
#include "raymath.h"
#include "resources.hpp"
#include "settings.hpp"

template <> void ECS::run_system<System::Tile>()
{
  Component_Registry<Tile> &tile_registry = component_registry<Tile>();
  Component_Registry<Collider> &body_registry = component_registry<Collider>();
  Component_Registry<View> &view_registry = component_registry<View>();

  for (const auto &view_id : view_registry.all_ids()) {
    auto &curr_view = view_registry.get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_body = body_registry.get(view_id);
    curr_view.camera.target = {curr_body.bound.x, curr_body.bound.y};
    BeginMode2D(curr_view.camera);
    for (const auto &tile_id : tile_registry.all_ids()) {
      auto &tile = tile_registry.get(tile_id);
      Vector2 position = {body_registry.get(tile_id).bound.x, body_registry.get(tile_id).bound.y};
      float dist_x = std::abs(curr_view.camera.target.x - position.x);
      if (dist_x < Settings::SCREEN_WIDTH / 8) {
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
      // Update frame
      auto &curr_anim = anim_registry.get(anim_id);
      if (input_registry.has(anim_id) && input_registry.get(anim_id).changed) {
        curr_anim.settings = Resources::get_resource_manager().animation(curr_anim.name, input_registry.get(anim_id).key_pressed);
      }
      // Draw frame
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

template <> void ECS::run_system<System::Draw>()
{
  BeginDrawing();
  ClearBackground(BLACK);
  run_system<System::Tile>();
  run_system<System::Animation>();
  EndDrawing();
}

template <> void ECS::run_system<System::Physics>()
{
  Component_Registry<Collider> &body_registry = component_registry<Collider>();
  Component_Registry<Input> &input_registry = component_registry<Input>();

  for (const auto &id : input_registry.all_ids()) {
    auto movement = Settings::key_to_movement.at(input_registry.get(id).key_pressed);
    Rectangle updated_rect = Settings::move(body_registry.get(id).bound, movement);
    updated_rect.y += 1;
    for (const auto &body_id : body_registry.all_ids()) {
      if (body_id == id) {
        continue;
      }
      if (CheckCollisionRecs(updated_rect, body_registry.get(body_id).bound)) {
        // result depending on body types
        if (body_registry.get(id).type == Body_Type::Player) {
          if (body_registry.get(body_id).type == Body_Type::Enemy) {
          }
          else if (body_registry.get(body_id).type == Body_Type::Wall) {
            if (updated_rect.y < body_registry.get(body_id).bound.y) {
              updated_rect.y = body_registry.get(id).bound.y;
            }
            else {
              return;
            }
          }
          else if (body_registry.get(body_id).type == Body_Type::Door) {
          }
          else if (body_registry.get(body_id).type == Body_Type::Sprite) {
            break;
          }
        }
      }
    }
    body_registry.get(id).bound = updated_rect;
  }
}

static KeyboardKey key_down()
{
  if (IsKeyDown(KEY_RIGHT))
    return KEY_RIGHT;
  if (IsKeyDown(KEY_LEFT))
    return KEY_LEFT;
  if (IsKeyDown(KEY_SPACE))
    return KEY_SPACE;
  /* To Implement
  if (IsKeyDown(KEY_UP))
    return KEY_UP;
  if (IsKeyDown(KEY_DOWN))
    return KEY_DOWN;
  */
  return KEY_NULL;
}

template <> void ECS::run_system<System::Input>()
{
  Component_Registry<Input> &input_registry = component_registry<Input>();
  for (const auto &id : input_registry.all_ids()) {
    if (!input_registry.get(id).active) {
      continue;
    }
    KeyboardKey key_pressed = key_down();
    if (key_pressed == input_registry.get(id).key_pressed) {
      input_registry.get(id).changed = false;
    }
    else {
      input_registry.get(id).key_pressed = key_pressed;
      input_registry.get(id).changed = true;
    }
  }
}
