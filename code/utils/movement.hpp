#pragma once
#include <raylib.h>
#include "../utils/side.hpp"

Side get_collision_side(const Rectangle &rect, const Rectangle &other);
