#pragma once
#include "../utils/side.hpp"
#include <raylib.h>

Side get_collision_side(const Rectangle &rect, const Rectangle &other);
