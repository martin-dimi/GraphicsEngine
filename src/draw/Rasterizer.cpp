#include "Rasterizer.hpp"
#include <Colour.h>
#include <glm/glm.hpp>

#include "Utilities.h"
#include "DrawUtils.h"



// PUBLIC
namespace rasterizer {
void draw(World& world, DrawingWindow& window, bool showWireframe) 
{
     // Create a depth buffer
    float *depthBuffer = new float[window.width * window.height];
    std::fill_n(depthBuffer, window.width * window.height, std::numeric_limits<float>::infinity());

    for (ModelTriangle &modelTriangle : world.getMesh())
    {
        if(modelTriangle.vertices[0].z > 0.0f) continue;
        if(modelTriangle.vertices[1].z > 0.0f) continue;
        if(modelTriangle.vertices[2].z > 0.0f) continue;
        
        CanvasTriangle canvasTriangle = utilities::convertToCanvasTriangle(modelTriangle, world.camera, window);

        if (showWireframe)
            drawUtilities::drawTriangleOutline(canvasTriangle, window);
        else if (modelTriangle.isTextured)
            drawUtilities::drawTriangleTextured(canvasTriangle, world.texture, window, depthBuffer);
        else
            drawUtilities::drawTriangleFilled(canvasTriangle, window, depthBuffer);
    }

    delete [] depthBuffer;
}
}