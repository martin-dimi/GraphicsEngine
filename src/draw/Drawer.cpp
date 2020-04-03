#include "Drawer.hpp"
#include "Rasterizer.hpp"
#include "Raytracer.hpp"
#include "DrawUtils.h"

void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow& window)
{
    drawUtilities::drawLine(a, b, c, window);
}

void drawTriangle(CanvasTriangle triangle, bool isFilled, DrawingWindow& window)
{
    if(isFilled)
        drawUtilities::drawTriangleFilled(triangle, window, NULL);
    else
        drawUtilities::drawTriangleOutline(triangle, window);
}

void drawImage(PPMImage& image, DrawingWindow& window)
{
    drawUtilities::drawImage(image, window);
}

void drawModel(World& world, DrawingWindow& window, bool raytrace) 
{
    if(raytrace)
        raytracer::draw(world, window);
    else
        rasterizer::draw(world, window, false);
}

void drawModelWireframe(World& world, DrawingWindow& window)
{
    rasterizer::draw(world, window, true);
}
