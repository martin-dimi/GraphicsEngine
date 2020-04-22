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

        // Near/Far plane clipping
        float distance = glm::distance(world.camera.position * world.camera.orientation, modelTriangle.vertices[0]);
        if(distance > 35.0f || distance < 2.0f) continue;

        // Back culling
        glm::vec3 normal = modelTriangle.calculateNormal();
        glm::vec3 dirToCamera = glm::normalize(modelTriangle.vertices[1] - world.camera.position*world.camera.orientation);
        float dot = glm::dot(normal, dirToCamera);
        if(dot > 0) continue;

        
        CanvasTriangle canvasTriangle = utilities::convertToCanvasTriangle(modelTriangle, world.camera, window);
        if (showWireframe)
            drawUtilities::drawTriangleOutline(canvasTriangle, window);
        else if (modelTriangle.isTextured)
            drawUtilities::drawTriangleTextured(canvasTriangle, world.texture, window, depthBuffer);
        else
            drawUtilities::drawTriangleFilled(canvasTriangle, window, depthBuffer);
    }

    auto lightPoint = utilities::convertToCanvasPoint(world.light.location, world.camera, window);
    drawUtilities::drawPoint(lightPoint, window, Colour(255,255,0));

    delete [] depthBuffer;
}
}