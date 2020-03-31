#pragma once

#include "model/World.hpp"
#include <DrawingWindow.h>

namespace rasterizer
{
    void draw(World& world, DrawingWindow& window, bool isWireframe);
}