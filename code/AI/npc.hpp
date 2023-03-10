#ifndef AI_H_

#include <vector>
#include <raylib.h>

struct AI;

namespace NPC {
  std::vector<KeyboardKey> keys_down(AI &ai);
}

#endif // !AI_H_