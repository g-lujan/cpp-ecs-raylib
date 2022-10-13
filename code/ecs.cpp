#include "ecs.hpp"
#include "raylib.h"
#include "raymath.h"
#include "resources.hpp"
#include "settings.hpp"
#include <iostream>

template <> void ECS::run_system<System::Tile>()
{
  Component_Registry<Tile> &tile_registry = component_registry<Tile>();
  Component_Registry<Body> &body_registry = component_registry<Body>();
  Component_Registry<View> &view_registry = component_registry<View>();
  Component_Registry<Input> &input_registry = component_registry<Input>();

  for (const auto &view_id : view_registry.all_ids()) {
    auto &curr_view = view_registry.get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_body = body_registry.get(view_id);
    curr_view.camera.target = {curr_body.rect.x, curr_body.rect.y};
    BeginMode2D(curr_view.camera);
    for (const auto &tile_id : tile_registry.all_ids()) {
      // Update frame
      auto &tile_component = tile_registry.get(tile_id);
      auto &tile = Resources::tiles[tile_component.map][tile_component.tile_name];
      // Draw frame
      Vector2 position = {body_registry.get(tile_id).rect.x, body_registry.get(tile_id).rect.y};
      if (Vector2Distance(curr_view.camera.target, position) < Settings::SCREEN_WIDTH / 4) {
        tile.draw(position, curr_view.tint);
      }
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Animation>()
{
  Component_Registry<Anim> &anim_registry = component_registry<Anim>();
  Component_Registry<Body> &body_registry = component_registry<Body>();
  Component_Registry<View> &view_registry = component_registry<View>();
  Component_Registry<Input> &input_registry = component_registry<Input>();

  for (const auto &view_id : view_registry.all_ids()) {
    auto &curr_view = view_registry.get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_body = body_registry.get(view_id);
    curr_view.camera.target = {curr_body.rect.x, curr_body.rect.y};
    BeginMode2D(curr_view.camera);
    for (const auto &anim_id : anim_registry.all_ids_ordered()) {
      // Update frame
      auto &curr_anim = anim_registry.get(anim_id).anim;
      // if has input binded to this entity
      if (input_registry.has(anim_id) && input_registry.get(anim_id).changed) {
        curr_anim = Resources::animations[curr_anim.type()][input_registry.get(anim_id).key_pressed];
      }
      // Draw frame
      Vector2 position = {body_registry.get(anim_id).rect.x, body_registry.get(anim_id).rect.y};
      if (Vector2Distance(curr_view.camera.target, position) < Settings::SCREEN_WIDTH / 4) {
        curr_anim.run(position, curr_view.tint);
      }
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Physics>()
{
  Component_Registry<Body> &body_registry = component_registry<Body>();
  Component_Registry<Input> &input_registry = component_registry<Input>();

  for (const auto &id : input_registry.all_ids()) {
    body_registry.get(id).closest_x = FLT_MAX;
    body_registry.get(id).closest_y = FLT_MAX;
    auto movement = Settings::key_to_movement.at(input_registry.get(id).key_pressed);
    Rectangle updated_rect = Settings::move(body_registry.get(id).rect, movement);
    for (const auto &body_id : body_registry.all_ids()) {
      if (body_id == id) {
        continue;
      }
      if (CheckCollisionRecs(updated_rect, body_registry.get(body_id).rect)) {
        // result depending on body types
        if (body_registry.get(id).type == Body_Type::Player) {
          if (body_registry.get(body_id).type == Body_Type::Enemy) {
            // - decrease enemy and player health
            // - throw both back (potencialmente perigoso)
            // - don't commit the movement
            // idea: something like
            //    body_registry.get(body_id).apply_collision(body_registry.get(id));
            //    this way, you'll not need the if elses!!
          }
          else if (body_registry.get(body_id).type == Body_Type::Wall) {
            // OK, just ignore updated movement
          }
          else if (body_registry.get(body_id).type == Body_Type::Door) {
            // level change if has key
            // else just ignore
          }
          else if (body_registry.get(body_id).type == Body_Type::Sprite) {
            break;
          }
          return;
        }
      }
      else {
        const float curr_x_dist = std::abs(body_registry.get(id).rect.x - body_registry.get(body_id).rect.x);
        body_registry.get(id).closest_x = std::min(body_registry.get(id).closest_x, curr_x_dist);
        const float curr_y_dist = std::abs(body_registry.get(id).rect.y - body_registry.get(body_id).rect.y);
        body_registry.get(id).closest_y = std::min(body_registry.get(id).closest_y, curr_y_dist);
      }
    }
    body_registry.get(id).rect = updated_rect;
    std::cout << "closest: " << body_registry.get(id).closest_x << " " << body_registry.get(id).closest_y << " " << std::endl;
  }
}

static KeyboardKey key_down()
{
  if (IsKeyDown(KEY_RIGHT))
    return KEY_RIGHT;
  if (IsKeyDown(KEY_LEFT))
    return KEY_LEFT;
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
