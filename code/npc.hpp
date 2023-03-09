#ifndef AI_H_

#include <random>
#include <vector>

#include <raylib.h>

class AI;

namespace NPC {
  std::vector<KeyboardKey> keys_down(AI &ai);
}

#endif // !AI_H_