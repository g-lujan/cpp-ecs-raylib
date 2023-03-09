#include "npc.hpp"
#include "components.hpp"

std::vector<KeyboardKey> NPC::keys_down(AI &ai)
{
  float curr_time = GetTime();

  std::vector<KeyboardKey> keys;
  if (curr_time - ai.last_call > 1.f) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);
    unsigned int rand_value = dist9(rng);
    if (rand_value <= 3) {
      keys.push_back(KEY_LEFT);
      ai.last_action = KEY_LEFT;
    }
    if (rand_value > 3 && rand_value < 9) {
      keys.push_back(KEY_RIGHT);
      ai.last_action = KEY_RIGHT;
    }
    if (rand_value == 9) {
      keys.push_back(KEY_SPACE);
      ai.last_action = KEY_SPACE;
    }
    ai.last_call = GetTime();
  }
  else {
    keys.push_back(ai.last_action);
  }
  return keys;
}