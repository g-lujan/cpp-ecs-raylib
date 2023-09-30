#include "time.hpp"
#include <raylib.h>

constexpr float MIN_TIME_BETWEEN_FRAMES = 1 / 60.f;

void cap_framerate()
{
  const float elapsed = GetFrameTime();
  if (MIN_TIME_BETWEEN_FRAMES > elapsed) {
    WaitTime(MIN_TIME_BETWEEN_FRAMES - elapsed);
  }
}