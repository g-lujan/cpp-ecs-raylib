#ifndef AI_H_

#include <random>
#include <vector>

#include<raylib.h>

#include "ecs.hpp"

namespace NPC {
  std::vector<KeyboardKey> keys_down(AI &ai);
}

#endif // !AI_H_