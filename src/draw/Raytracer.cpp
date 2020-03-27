#include <RayTriangleIntersection.h>
#include <Colour.h>
#include <glm/glm.hpp>
#include "model/Ray.hpp"
#include "Utilities.h"
#include "external/OBJFile.h"

void raytrace(int x, int y, DrawingWindow window, Camera camera, OBJFile model);

// PUBLIC
namespace raytracer
{
void draw(OBJFile model, Camera camera, DrawingWindow window)
{
    for (int row = 0; row < window.height; row++)
        for (int col = 0; col < window.width; col++)
            raytrace(col, row, window, camera, model);
}
}

// PRIVATE
void raytrace(int x, int y, DrawingWindow window, Camera camera, OBJFile model)
{
    // Calculate ray
    float rayX = (x - window.halfWidth)  / window.scale;
    float rayY = (window.halfHeight - y) / window.scale;
    float rayZ = -camera.f;

    glm::vec3 rayDir = glm::normalize(glm::vec3(rayX, rayY, rayZ));
    Ray ray = Ray(camera.position, rayDir);

    // Fire ray
    RayTriangleIntersection intersection = utilities::getClosestIntersection(camera, ray, model.loadedFaces, -1);

    if(intersection.hasHit)
    {
        glm::vec3 light = (model.lightSource.getLocation() - camera.position) * camera.orientation + camera.position;
        glm::vec3 triangleNormal = intersection.intersectedTriangle.calculateNormal();
        glm::vec3 dirToLight     = glm::normalize(light - intersection.intersectionPoint);

        // Proximity lighting
        float distanceToLight = glm::distance(intersection.intersectionPoint, light);
        float brightness = model.lightSource.getIntensity() / (4 * 3.14f * distanceToLight * distanceToLight);
        if(brightness > 1.0) brightness = 1.0f;

        // Angle of incidence
        float angle = glm::dot(dirToLight, triangleNormal);
        brightness *= angle;

        // Ambient light
        if(brightness <= 0.2f) brightness = 0.2f;

        // Shadows
        Ray shadowRay = Ray(intersection.intersectionPoint, dirToLight);
        RayTriangleIntersection shadowIntersection = utilities::getClosestIntersection(camera, shadowRay, model.loadedFaces, intersection.intersectedTriangle.id);
        float distanceToIntersection = glm::distance(intersection.intersectionPoint, shadowIntersection.intersectionPoint);
        if(shadowIntersection.hasHit && distanceToIntersection <= distanceToLight) 
            brightness = 0.15f;
        
        window.setPixelColour(x, y, intersection.intersectedTriangle.colour.getPackedInt(brightness));
    }
    else
        window.setPixelColour(x, y, window.backgroundColour.getPackedInt());  
}