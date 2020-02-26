#pragma once

#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include <glm/glm.hpp>
#include "PPMImage.h"
#include "OBJFile.h"
#include "Camera.hpp"

extern const int HEIGHT;
extern const int WIDTH;

void drawRedError(DrawingWindow window);
void drawGrayGradient(DrawingWindow window);
void drawColorGradient(DrawingWindow window);

void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow window);
void drawTriangleOutline(CanvasTriangle triangle, Colour c, DrawingWindow window);
void drawTriangleFilled(CanvasTriangle triangle, Colour c, DrawingWindow window);

void loadModel(OBJFile model, Camera camera, DrawingWindow window);
void loadImage(PPMImage image, DrawingWindow window);
void drawTriangleImageFilled(CanvasTriangle triangle, PPMImage image, DrawingWindow window);