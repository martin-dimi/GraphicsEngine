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
    void drawImage(PPMImage image, DrawingWindow window);
    void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow& window);
    void drawTriangleOutline(CanvasTriangle& triangle, DrawingWindow& window);
    void drawTriangleFilled(CanvasTriangle& triangle, DrawingWindow& window, float *depthBuffer);
    void drawTriangleTextured(CanvasTriangle& triangle, PPMImage& image, DrawingWindow& window, float *depthBuffer);
}