#include "Raytracer.hpp"
#include <Colour.h>
#include <glm/glm.hpp>
#include "model/Ray.hpp"
#include "model/RayIntersection.hpp"
#include "model/Camera.hpp"

void raytrace(int x, int y, DrawingWindow& window, World& world);
Ray calculateRay(float x, float y, DrawingWindow& window, Camera& camera);
RayIntersection fireRay(Ray& ray, World& world);
RayIntersection getClosestIntersection(Camera& camera, Ray& ray, World& world, int ignoreId);
RayIntersection noAliasing(float x, float y, DrawingWindow& window, World& world);
RayIntersection aliasQuincunx(float x, float y, DrawingWindow& window, World& world);
RayIntersection aliasRGSS(float x, float y, DrawingWindow& window, World& world);

// PUBLIC
namespace raytracer
{
void draw(World& world, DrawingWindow& window)
{
    for (int row = 0; row < window.height; row++) 
    {
        std::cout << "Raytracing row: " << row << std::endl;
        for (int col = 0; col < window.width; col++)
            raytrace(col, row, window, world);
    }
    std::cout << "Raytracing finished." << std::endl;

}
}

// PRIVATE
void raytrace(int x, int y, DrawingWindow& window, World& world)
{
    // Select aliasing...
    RayIntersection intersection = aliasRGSS(x, y, window, world);
    // RayIntersection intersection = aliasQuincunx(x, y, window, world);
    // RayIntersection intersection = noAliasing(x, y, window, world);

    if(intersection.hasHit)
        window.setPixelColour(x, y, intersection.intersectionColour.getPackedInt(intersection.intersectionBrightness));
    else
        window.setPixelColour(x, y, window.backgroundColour.getPackedInt());  
}

RayIntersection fireRay(Ray& ray, World& world)
{
    RayIntersection intersection = getClosestIntersection(world.camera, ray, world, -1);

    // Calculate light if the ray has hit something
    if(intersection.hasHit)
    {
        glm::vec3 light = (world.light.location - world.camera.position) * world.camera.orientation + world.camera.position;
        glm::vec3 triangleNormal = intersection.intersectionNormal;
        glm::vec3 dirToLight = glm::normalize(light - intersection.intersectionPoint);

        // Proximity lighting
        float distanceToLight = glm::distance(intersection.intersectionPoint, light);
        float brightness = world.light.intensity / (4 * 3.14f * distanceToLight * distanceToLight);
        if(brightness > 1.0) brightness = 1.0f;

        // Angle of incidence
        float angle = glm::dot(dirToLight, triangleNormal);
        brightness *= angle;

        // Ambient light
        if(brightness <= 0.2f) brightness = 0.2f;

        // Shadows
        Ray shadowRay = Ray(intersection.intersectionPoint, dirToLight);
        RayIntersection shadowIntersection = getClosestIntersection(world.camera, shadowRay, world, intersection.intersectedTriangle.id);
        float distanceToIntersection = glm::distance(intersection.intersectionPoint, shadowIntersection.intersectionPoint);
        if(shadowIntersection.hasHit && distanceToIntersection <= distanceToLight) 
            brightness = 0.2f;

        intersection.intersectionBrightness = brightness;
    }

    return intersection;
}

RayIntersection getClosestIntersection(Camera& camera, Ray& ray, World& world, int ignoreId)
{
    RayIntersection closestIntersection;
    float intU, intV;

    // Go through each triangle and check if the ray is intersecting
    for (ModelTriangle triangle : world.getMesh())
    {
        if(ignoreId == triangle.id) continue;
        
        glm::vec3 v0 = triangle.vertices[0] + camera.position;
        glm::vec3 v1 = triangle.vertices[1] + camera.position;
        glm::vec3 v2 = triangle.vertices[2] + camera.position;

        vec3 e0 = v1 - v0;
        vec3 e1 = v2 - v0;
        vec3 SPVector = ray.getStart() - v0;
        mat3 DEMatrix = mat3(-ray.getDirection(), e0, e1);

        // [0] - the distance along the ray from the camera to the intersection point
        // [1] - the proportion along the triangle's first edge that the intersection point appears
        // [2] - the proportion along the triangle's second edge that the intersection point appears
        vec3 solution = inverse(DEMatrix) * SPVector;
        float distance = solution[0];
        float u = solution[1];
        float v = solution[2];

        // verify there is an intersection
        if (distance < 0 || u < 0 || v < 0 || v + u > 1)
            continue;

        if (closestIntersection.distance > distance)
        {  
            closestIntersection.hasHit = true;
            closestIntersection.distance = distance;
            closestIntersection.intersectionPoint = ray.getStart() + distance * ray.getDirection();
            closestIntersection.intersectedTriangle = triangle;

            intU = u;
            intV = v;
        }
    }

    if(closestIntersection.hasHit) 
    {
        ModelTriangle &triangle = closestIntersection.intersectedTriangle;

        // Calculate normal
        if(triangle.hasNormals)
        {
            vec3 normal1 = triangle.normals[1]-triangle.normals[0];
            vec3 normal2 = triangle.normals[2]-triangle.normals[0];
            closestIntersection.intersectionNormal = normalize(triangle.normals[0] + (normal1 * intU) + (normal2 * intV));
        } else 
            closestIntersection.intersectionNormal = triangle.calculateNormal();

        // Calculate colour
        if(triangle.isTextured)
        {
            vec2 te0 = triangle.texture[1] - triangle.texture[0];
            vec2 te1 = triangle.texture[2] - triangle.texture[0];
            vec2 textureIntersection = triangle.texture[0] + (te0 * intU) + (te1 * intV);

            Colour texture = world.texture.getPixelValueAt(textureIntersection);
            closestIntersection.intersectionColour = texture;
        } else 
            closestIntersection.intersectionColour = Colour(triangle.colour);
    }
   


    return closestIntersection;
}

RayIntersection noAliasing(float x, float y, DrawingWindow& window, World& world)
{
    // Calculate ray
    Ray ray = calculateRay(x + 0.5f, y + 0.5f, window, world.camera);

    // Fire ray
    return fireRay(ray, world);
}

RayIntersection aliasQuincunx(float x, float y, DrawingWindow& window, World& world)
{
    vector<Ray> rays;

    // Calculate ray
    rays.push_back(calculateRay(x + 0.5f, y + 0.5f, window, world.camera));
    rays.push_back(calculateRay(x, y, window, world.camera));
    rays.push_back(calculateRay(x, y + 1.0f, window, world.camera));
    rays.push_back(calculateRay(x + 1.0f, y, window, world.camera));
    rays.push_back(calculateRay(x + 1.0f, y+1.0f, window, world.camera));

    int haveHit = 0;
    Colour colour = Colour(0,0,0);
    float brightness = 0.0f;
    RayIntersection middle;

    for(int i=0; i<rays.size(); i++)
    {
        // Fire ray
        Ray ray = rays[i];
        RayIntersection intersection = fireRay(ray, world);

        if(i==0) 
            middle = intersection; 

        if(intersection.hasHit)
        {
            haveHit++;

            float w = 0.125f;
            if(i==0) w = 0.5f;

            colour.red += w * intersection.intersectionColour.red;
            colour.green += w * intersection.intersectionColour.green;
            colour.blue += w * intersection.intersectionColour.blue;
            brightness += w * intersection.intersectionBrightness;
        }
    }

    if(haveHit > 0)
    {
        middle.intersectionColour = colour;
        middle.intersectionBrightness = brightness;
    } 

    return middle;        
}

RayIntersection aliasRGSS(float x, float y, DrawingWindow& window, World& world)
{
    vector<Ray> rays;
    float offset = 0.125f;
    float w = 0.25f;

    // Calculate ray
    rays.push_back(calculateRay(x + 5*offset, y + 1*offset, window, world.camera));
    rays.push_back(calculateRay(x + 1*offset, y + 3*offset, window, world.camera));
    rays.push_back(calculateRay(x + 7*offset, y + 5*offset, window, world.camera));
    rays.push_back(calculateRay(x + 3*offset, y + 7*offset, window, world.camera));

    int haveHit = 0;
    Colour colour = Colour(0,0,0);
    float brightness = 0.0f;
    RayIntersection middle;

    for(int i=0; i<rays.size(); i++)
    {
        // Fire ray
        Ray ray = rays[i];
        RayIntersection intersection = fireRay(ray, world);

        if(i==0) 
            middle = intersection; 

        if(intersection.hasHit)
        {
            haveHit++;

            colour.red += w * intersection.intersectionColour.red;
            colour.green += w * intersection.intersectionColour.green;
            colour.blue += w * intersection.intersectionColour.blue;
            brightness += w * intersection.intersectionBrightness;
        }
    }

    if(haveHit > 0)
    {
        middle.intersectionColour = colour;
        middle.intersectionBrightness = brightness;
    } 

    return middle;        
}

Ray calculateRay(float x, float y, DrawingWindow& window, Camera& camera)
{
    float rayX = (x - window.halfWidth)  / window.scale;
    float rayY = (window.halfHeight - y) / window.scale;
    float rayZ = -camera.f;

    glm::vec3 rayDir = glm::normalize(glm::vec3(rayX, rayY, rayZ));
    return Ray(camera.position, rayDir);
}