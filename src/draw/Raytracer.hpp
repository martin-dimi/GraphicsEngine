#pragma once

#include "external/OBJFile.h"
#include "model/Camera.hpp"
#include <DrawingWindow.h>

namespace raytracer
{
    void draw(OBJFile& model, Camera camera, DrawingWindow window);
}