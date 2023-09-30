#ifndef AI_H_

#include <raylib.h>
#include <vector>

struct AI;

namespace NPC {
  std::vector<KeyboardKey> keys_down(AI &ai);
}

#endif // !AI_H_