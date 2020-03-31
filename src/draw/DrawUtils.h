#pragma once

#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include <glm/glm.hpp>
#include "external/PPMImage.h"
#include "external/OBJFile.h"
#include "model/Camera.hpp"

namespace drawUtilities
{
    void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow window);
    void drawTriangle(CanvasTriangle& triangle, bool isFilled, DrawingWindow& window, float *depthBuffer);
    void drawTriangleTexture(CanvasTriangle triangle, PPMImage image, DrawingWindow window);
    void drawImage(PPMImage image, DrawingWindow window);
}


