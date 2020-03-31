#pragma once

#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include "external/PPMImage.h"
#include "external/OBJFile.h"
#include "model/Camera.hpp"

void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow window);
void drawTriangle(CanvasTriangle triangle, bool isFilled, DrawingWindow window);

void drawImage(PPMImage& image, DrawingWindow window);
void drawModel(OBJFile& model, Camera camera, DrawingWindow window, bool raytrace);
void drawModelWireframe(OBJFile& model, Camera camera, DrawingWindow window);
