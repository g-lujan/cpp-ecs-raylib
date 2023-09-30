#include "ecs.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

#include "../utils/movement.hpp"
#include "../AI/npc.hpp"
#include "../utils/time.hpp"
#include "../management/settings.hpp"
#include "../management/resources.hpp"
#include "../components/all_components.hpp"
#include "../management/serialization.hpp"
#include "../../external/json.hpp"
#include "../systems/available_systems.hpp"

template <> void ECS::run_system<System::Tile>()
{
  Component_Registry<Tile> *tile_registry = component_registry<Tile>();
  Component_Registry<Kinematics> *kinematics_registry = component_registry<Kinematics>();
  Component_Registry<View> *view_registry = component_registry<View>();

  for (const auto &view_id : view_registry->all_ids()) {
    auto &curr_view = view_registry->get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_pos = kinematics_registry->get(view_id).position;
    // update camera
    curr_view.camera.target = {curr_pos.x, curr_pos.y};
    BeginMode2D(curr_view.camera);
    // draw tiles
    for (const auto &tile_id : tile_registry->all_ids()) {
      auto &tile = tile_registry->get(tile_id);
      tile.tex->draw(tile.src_rect, kinematics_registry->get(tile_id).position, curr_view.tint, tile.rotation, tile.flip);
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Animation>()
{
  Component_Registry<Anim> *anim_registry = component_registry<Anim>();
  Component_Registry<Collider> *body_registry = component_registry<Collider>();
  Component_Registry<View> *view_registry = component_registry<View>();

  for (const auto &view_id : view_registry->all_ids()) {
    auto &curr_view = view_registry->get(view_id);
    if (!curr_view.active) {
      continue;
    }
    auto &curr_body = body_registry->get(view_id);
    curr_view.camera.target = {curr_body.bound.x, curr_body.bound.y};
    BeginMode2D(curr_view.camera);
    for (const auto &anim_id : anim_registry->all_ids_ordered()) {
      auto &curr_anim = anim_registry->get(anim_id);
      Vector2 position = {body_registry->get(anim_id).bound.x, body_registry->get(anim_id).bound.y};
      float dist_x = std::abs(curr_view.camera.target.x - position.x);
      if (dist_x < Settings::SCREEN_WIDTH / 4) {
        curr_anim.frame.step(curr_anim.frame.action != "jump" ? 0.f : 32.f);
        Resources::get_resource_manager()
            .texture(curr_anim.texture_id)
            .draw(curr_anim.frame.src_rect, position, curr_view.tint, 0.f, curr_anim.flip);
      }
    }
    EndMode2D();
  }
}

template <> void ECS::run_system<System::Player_Animation>()
{
  Component_Registry<Player> *player_registry = component_registry<Player>();
  Component_Registry<Anim> *anim_registry = component_registry<Anim>();
  Component_Registry<Input> *input_registry = component_registry<Input>();
  Component_Registry<Kinematics> *kinematics_registry = component_registry<Kinematics>();
  Component_Registry<Controls> *controls_registry = component_registry<Controls>();

  for (const auto &player_id : player_registry->all_ids()) {
    auto &curr_anim = anim_registry->get(player_id);
    auto &keys_pressed = input_registry->get(player_id).keys_pressed;
    auto &player_controls = controls_registry->get(player_id);
    auto &kinematics = kinematics_registry->get(player_id);
    curr_anim.flip.horizontally = kinematics.velocity.x < 0.f ? true : kinematics.velocity.x > 0.f ? false : curr_anim.flip.horizontally;

    if (kinematics.velocity.y != 0.f) {
      auto &jump_anim_settings = Resources::get_resource_manager().animation(curr_anim.name, "jump");
      // check if anim has changed
      if (jump_anim_settings.start_pos.x != curr_anim.frame.start_pos.x ||
          jump_anim_settings.start_pos.y != curr_anim.frame.start_pos.y) {
        // update animation
        curr_anim.frame = jump_anim_settings;
      }
      return;
    }
    else if (keys_pressed.empty()) {
      curr_anim.frame = Resources::get_resource_manager().animation(curr_anim.name, player_controls.key_to_movement[KEY_NULL].action_id);
    }

    for (KeyboardKey &key : keys_pressed) {
      auto &new_anim_settings = Resources::get_resource_manager().animation(curr_anim.name, player_controls.key_to_movement[key].action_id);
      // check if anim has changed
      if (new_anim_settings.start_pos.x != curr_anim.frame.start_pos.x ||
          new_anim_settings.start_pos.y != curr_anim.frame.start_pos.y) {
        // update animation
        curr_anim.frame = Resources::get_resource_manager().animation(curr_anim.name, player_controls.key_to_movement[key].action_id);
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

static void step_kinematics(Kinematics &kinematics);

template <> void ECS::run_system<System::Player_Movement>()
{
  Component_Registry<Player> *player_registry = component_registry<Player>();
  Component_Registry<Kinematics> *kinematics_registry = component_registry<Kinematics>();
  Component_Registry<Collider> *body_registry = component_registry<Collider>();
  Component_Registry<Input> *input_registry = component_registry<Input>();
  Component_Registry<Controls> *controls_registry = component_registry<Controls>();

  for (const auto &id : player_registry->all_ids()) {
    auto &player_body = body_registry->get(id);
    auto &keys_pressed = input_registry->get(id).keys_pressed;
    auto &kinematics = kinematics_registry->get(id);
    auto &controls = controls_registry->get(id);
    kinematics.velocity.x = 0;
    for (const KeyboardKey &key : keys_pressed) {
      controls.key_to_movement.at(key).action(kinematics, player_body.collision_sides);
    }
    step_kinematics(kinematics);
    player_body.bound.x = kinematics.position.x;
    player_body.bound.y = kinematics.position.y;
  }
}

static void step_kinematics(Kinematics &kinematics)
{
  const float dt = GetFrameTime();
  kinematics.position.x += kinematics.velocity.x * dt + (kinematics.acceleration.x * dt * dt) / 2;
  kinematics.velocity.x += kinematics.acceleration.x * dt;
  kinematics.position.y += kinematics.velocity.y * dt + (kinematics.acceleration.y * dt * dt) / 2;
  const float TERMINAL_VELOCITY = 100.f;
  kinematics.velocity.y += kinematics.velocity.y < TERMINAL_VELOCITY? 3 * kinematics.acceleration.y * dt : 0.f;
};

template <> void ECS::run_system<System::Physics>()
{
  Component_Registry<Collider> *body_registry = component_registry<Collider>();
  Component_Registry<Kinematics> *kinematics_registry = component_registry<Kinematics>();

  for (const auto &id : body_registry->all_ids()) {
    auto &body = body_registry->get(id);
    if (!kinematics_registry->has(id)) {
      continue;
    }
    // register collisions
    body.collision_sides.clear();
    for (const auto &other_id : body_registry->all_ids()) {
      if (other_id == id) {
        continue;
      }
      auto &other = body_registry->get(other_id);
      Side collision_side = get_collision_side(body.bound, other.bound);
      if (collision_side != Side::NONE) {
        body.collision_sides.insert(collision_side);
      }
    }
    // gravity
    auto &kinematics = kinematics_registry->get(id);
    if (!body.collision_sides.contains(Side::BOTTON)) {
      kinematics.acceleration.y = 70;
    }
    else {
      kinematics.acceleration.y = 0;
      kinematics.velocity.y = 0;
    }
  }
}

void push_if_keydown(KeyboardKey key, std::vector<KeyboardKey> &keys) {
  if (IsKeyDown(key)) {
    keys.push_back(key);
  }
}

void update_keys_down(std::vector<KeyboardKey> &keys)
{
  keys.clear();
  push_if_keydown(KEY_LEFT, keys);
  push_if_keydown(KEY_RIGHT, keys);
  push_if_keydown(KEY_SPACE, keys);
  push_if_keydown(KEY_ENTER, keys);
  push_if_keydown(KEY_A, keys);
  push_if_keydown(KEY_W, keys);
  push_if_keydown(KEY_D, keys);
}

// register input
template <> void ECS::run_system<System::Input>()
{
  Component_Registry<Input> *input_registry = component_registry<Input>();
  for (const auto &id : input_registry->all_ids()) {
    if (!input_registry->get(id).active) {
      continue;
    }
    update_keys_down(input_registry->get(id).keys_pressed);
  }
}

template <> void ECS::run_system<System::AI>() {
  Component_Registry<Input> *input_registry = component_registry<Input>();
  Component_Registry<AI> *ai_registry = component_registry<AI>();

  for (const auto &id : ai_registry->all_ids()) {
    input_registry->get(id).keys_pressed = NPC::keys_down(ai_registry->get(id));
  }
}


template <> void ECS::run_system<System::InGameMenu>()
{ 
    serialize();
    Component_Registry<Input> *input_registry = component_registry<Input>();
    Component_Registry<Player> *player_registry = component_registry<Player>();
    ECS subsystem;
    for (const auto &id : player_registry->all_ids()) {
        const auto &keys_pressed = input_registry->get(id).keys_pressed;
        if (std::find(keys_pressed.begin(), keys_pressed.end(), KEY_ENTER) == keys_pressed.end()) {
            continue;
        }
        unsigned long long menu_id = subsystem.spawn_entity(Input(true));
        Component_Registry<Input> *menu_input_registry = subsystem.component_registry<Input>();
        // it's flickering
        while (!WindowShouldClose()) {
            cap_framerate();
            BeginDrawing();
            auto &menu_keys_pressed = menu_input_registry->get(menu_id).keys_pressed;
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

    for (auto &&[id, to_purge] : _entities) {
        if (to_purge) {
            continue;
        }
        for (auto &&[type, registry] : _component_registries) {
            serialize_component(id, output, registry.get());
        }
    }
    save(output);
}