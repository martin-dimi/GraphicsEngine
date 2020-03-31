#pragma once

#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include "external/PPMImage.h"
#include "model/World.hpp"

void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow& window);
void drawTriangle(CanvasTriangle triangle, bool isFilled, DrawingWindow& window);

void drawImage(PPMImage& image, DrawingWindow& window);
void drawModel(World& world, DrawingWindow& window, bool raytrace);
void drawModelWireframe(World& world, DrawingWindow& window);
