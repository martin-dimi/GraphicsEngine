#include "Rasterizer.hpp"
#include "Raytracer.hpp"
#include "DrawUtils.h"


void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow window)
{
    drawUtilities::drawLine(a, b, c, window);
}

void drawTriangle(CanvasTriangle triangle, bool isFilled, DrawingWindow window)
{
    drawUtilities::drawTriangle(triangle, isFilled, window, NULL);
}

void drawImage(PPMImage& image, DrawingWindow window)
{
    drawUtilities::drawImage(image, window);
}

void drawModel(OBJFile& model, Camera camera, DrawingWindow window, bool raytrace) 
{
    if(raytrace)
        raytracer::draw(model, camera, window);
    else
        rasterizer::draw(model, camera, window, false);
}

void drawModelWireframe(OBJFile& model, Camera camera, DrawingWindow window)
{
    rasterizer::draw(model, camera, window, true);
}
