#pragma once

#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <RayTriangleIntersection.h>
#include <Colour.h>
#include <glm/glm.hpp>
#include "PPMImage.h"
#include "OBJFile.h"
#include "Camera.hpp"

void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow window);
void drawTriangle(CanvasTriangle triangle, bool isFilled, DrawingWindow window);
void drawTriangleTexture(CanvasTriangle triangle, PPMImage image, DrawingWindow window);

void loadModel(OBJFile model, Camera camera, DrawingWindow window, bool showWireframe);
void loadImage(PPMImage image, DrawingWindow window);
void raytrace(OBJFile model, Camera camera, DrawingWindow window);