#include <Colour.h>
#include <glm/glm.hpp>
#include "model/Ray.hpp"
#include "model/RayIntersection.hpp"
#include "Utilities.h"
#include "external/OBJFile.h"

void raytrace(int x, int y, DrawingWindow window, Camera camera, OBJFile& model);
Ray calculateRay(float x, float y, DrawingWindow window, Camera camera);
RayIntersection fireRay(Ray ray, Camera camera, OBJFile& model);
RayIntersection noAliasing(float x, float y, DrawingWindow window, Camera camera, OBJFile& model);
RayIntersection aliasQuincunx(float x, float y, DrawingWindow window, Camera camera, OBJFile& model);
RayIntersection aliasRGSS(float x, float y, DrawingWindow window, Camera camera, OBJFile& model);

// PUBLIC
namespace raytracer
{
void draw(OBJFile& model, Camera camera, DrawingWindow window)
{
    for (int row = 0; row < window.height; row++)
        for (int col = 0; col < window.width; col++)
            raytrace(col, row, window, camera, model);
}
}

// PRIVATE
void raytrace(int x, int y, DrawingWindow window, Camera camera, OBJFile& model)
{
    // std::cout << "RayTracing X:" << x << ", Y: " << y << std::endl;
    RayIntersection intersection = aliasRGSS(x, y, window, camera, model);

    if(intersection.hasHit)
        window.setPixelColour(x, y, intersection.intersectionColour.getPackedInt(intersection.intersectionBrightness));
    else
        window.setPixelColour(x, y, window.backgroundColour.getPackedInt());  
}

RayIntersection fireRay(Ray ray, Camera camera, OBJFile& model)
{
    RayIntersection intersection = utilities::getClosestIntersection(camera, ray, model.loadedFaces, -1);

    // Calculate light if the ray has hit something
    if(intersection.hasHit)
    {
        glm::vec3 light = (model.lightSource.location - camera.position) * camera.orientation + camera.position;
        glm::vec3 triangleNormal = intersection.intersectedTriangle.calculateNormal();
        glm::vec3 dirToLight     = glm::normalize(light - intersection.intersectionPoint);

        // Proximity lighting
        float distanceToLight = glm::distance(intersection.intersectionPoint, light);
        float brightness = model.lightSource.intensity / (4 * 3.14f * distanceToLight * distanceToLight);
        if(brightness > 1.0) brightness = 1.0f;

        // Angle of incidence
        float angle = glm::dot(dirToLight, triangleNormal);
        brightness *= angle;

        // Ambient light
        if(brightness <= 0.2f) brightness = 0.2f;

        // Shadows
        Ray shadowRay = Ray(intersection.intersectionPoint, dirToLight);
        RayIntersection shadowIntersection = utilities::getClosestIntersection(camera, shadowRay, model.loadedFaces, intersection.intersectedTriangle.id);
        float distanceToIntersection = glm::distance(intersection.intersectionPoint, shadowIntersection.intersectionPoint);
        if(shadowIntersection.hasHit && distanceToIntersection <= distanceToLight) 
            brightness = 0.2f;

        intersection.intersectionBrightness = brightness;
    }

    return intersection;
}

RayIntersection noAliasing(float x, float y, DrawingWindow window, Camera camera, OBJFile& model)
{
    // Calculate ray
    Ray ray = calculateRay(x + 0.5f, y + 0.5f, window, camera);

    // Fire ray
    return fireRay(ray, camera, model);
}

RayIntersection aliasQuincunx(float x, float y, DrawingWindow window, Camera camera, OBJFile& model)
{
    vector<Ray> rays;

    // Calculate ray
    rays.push_back(calculateRay(x + 0.5f, y + 0.5f, window, camera));
    rays.push_back(calculateRay(x, y, window, camera));
    rays.push_back(calculateRay(x, y + 1.0f, window, camera));
    rays.push_back(calculateRay(x + 1.0f, y, window, camera));
    rays.push_back(calculateRay(x + 1.0f, y+1.0f, window, camera));

    int haveHit = 0;
    Colour colour = Colour(0,0,0);
    float brightness = 0.0f;
    RayIntersection middle;

    for(int i=0; i<rays.size(); i++)
    {
        // Fire ray
        Ray ray = rays[i];
        RayIntersection intersection = fireRay(ray, camera, model);

        if(i==0) 
            middle = intersection; 

        if(intersection.hasHit)
        {
            haveHit++;

            float w = 0.125f;
            if(i==0) w = 0.5f;

            colour.red += w * intersection.intersectedTriangle.colour.red;
            colour.green += w * intersection.intersectedTriangle.colour.green;
            colour.blue += w * intersection.intersectedTriangle.colour.blue;
            brightness += w * intersection.intersectionBrightness;
        }
    }

    if(haveHit > 0)
    {
        // colour.red = std::round(colour.red / haveHit);
        // colour.green = std::round(colour.green / haveHit);
        // colour.blue = std::round(colour.blue / haveHit);

        middle.intersectionColour = colour;
        middle.intersectionBrightness = brightness;
    } 

    return middle;        
}

RayIntersection aliasRGSS(float x, float y, DrawingWindow window, Camera camera, OBJFile& model)
{
    vector<Ray> rays;
    float offset = 0.125f;
    float w = 0.25f;

    // Calculate ray
    rays.push_back(calculateRay(x + 5*offset, y + 1*offset, window, camera));
    rays.push_back(calculateRay(x + 1*offset, y + 3*offset, window, camera));
    rays.push_back(calculateRay(x + 7*offset, y + 5*offset, window, camera));
    rays.push_back(calculateRay(x + 3*offset, y + 7*offset, window, camera));

    int haveHit = 0;
    Colour colour = Colour(0,0,0);
    float brightness = 0.0f;
    RayIntersection middle;

    for(int i=0; i<rays.size(); i++)
    {
        // Fire ray
        Ray ray = rays[i];
        RayIntersection intersection = fireRay(ray, camera, model);

        if(i==0) 
            middle = intersection; 

        if(intersection.hasHit)
        {
            haveHit++;

            colour.red += w * intersection.intersectedTriangle.colour.red;
            colour.green += w * intersection.intersectedTriangle.colour.green;
            colour.blue += w * intersection.intersectedTriangle.colour.blue;
            brightness += w * intersection.intersectionBrightness;
        }
    }

    if(haveHit > 0)
    {
        // colour.red = std::round(colour.red / haveHit);
        // colour.green = std::round(colour.green / haveHit);
        // colour.blue = std::round(colour.blue / haveHit);

        middle.intersectionColour = colour;
        middle.intersectionBrightness = brightness;
    } 

    return middle;        
}

Ray calculateRay(float x, float y, DrawingWindow window, Camera camera)
{
    float rayX = (x - window.halfWidth)  / window.scale;
    float rayY = (window.halfHeight - y) / window.scale;
    float rayZ = -camera.f;

    glm::vec3 rayDir = glm::normalize(glm::vec3(rayX, rayY, rayZ));
    return Ray(camera.position, rayDir);
}