#pragma once

#include "external/OBJFile.h"
#include "model/Camera.hpp"
#include <DrawingWindow.h>

namespace rasterizer
{
    void draw(OBJFile model, Camera camera, DrawingWindow window, bool isWireframe);
}