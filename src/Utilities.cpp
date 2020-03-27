#include "Utilities.h"
#include <sstream>

CanvasPoint convertToCanvasPoint(glm::vec3 point, Camera camera, DrawingWindow window);

namespace utilities
{
std::vector<float> interpolate(float from, float to, int steps)
{
    std::vector<float> numbers;

    if (steps - 1 == 0)
        return numbers;

    float diff = to - from;
    float increment = diff / (steps - 1);

    for (float step = 0; step < steps; step += 1)
        numbers.push_back(from + step * increment);

    return numbers;
}

std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to)
{
    std::vector<CanvasPoint> numbers;

    int xDiff = to.x - from.x;
    int yDiff = to.y - from.y;

    float steps = max(abs(xDiff), abs(yDiff));

    float xStepSize = xDiff / steps;
    float yStepSize = yDiff / steps;

    for (int step = 0; step < steps; step++)
    {
        float x = from.x + step * xStepSize;
        float y = from.y + step * yStepSize;

        numbers.push_back(CanvasPoint(x, y));
    }

    return numbers;
}

std::vector<vec3> interpolate(glm::vec3 from, glm::vec3 to, int steps)
{
    std::vector<vec3> numbers;
    vec3 diff = to - from;
    vec3 increment = steps == 1 ? diff : diff / float(steps - 1);

    for (float step = 0; step <= steps; step += 1)
    {
        vec3 cur = from + step * increment;
        numbers.push_back(cur);
    }

    return numbers;
}

std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to, int steps)
{
    std::vector<CanvasPoint> points;

    if (steps <= 0)
        return points;

    float xDiff = to.x - from.x;
    float yDiff = to.y - from.y;
    float dDiff = to.depth - from.depth;

    float xTexDiff = to.texturePoint.x - from.texturePoint.x;
    float yTexDiff = to.texturePoint.y - from.texturePoint.y;

    float xStepSize = steps == 1 ? xDiff : xDiff / (steps - 1);
    float yStepSize = steps == 1 ? yDiff : yDiff / (steps - 1);
    float dStepSize = steps == 1 ? dDiff : dDiff / (steps - 1);

    float xTexStepSize = steps == 1 ? xTexDiff : xTexDiff / (steps - 1);
    float yTexStepSize = steps == 1 ? yTexDiff : yTexDiff / (steps - 1);

    for (float step = 0; step < steps; step += 1)
    {
        float curX = from.x + step * xStepSize;
        float curY = from.y + step * yStepSize;
        float curD = from.depth + step * dStepSize;

        float curTexX = from.texturePoint.x + step * xTexStepSize;
        float curTexY = from.texturePoint.y + step * yTexStepSize;

        CanvasPoint cur = CanvasPoint(curX, curY, curD);
        cur.texturePoint = TexturePoint(curTexX, curTexY);

        points.push_back(cur);
    }

    return points;
}

CanvasPoint getTriangleMidPoint(CanvasTriangle triangle)
{
    float heightDiff = triangle.vertices[2].y - triangle.vertices[0].y;
    float widthDiff = triangle.vertices[2].x - triangle.vertices[0].x;
    float depthDiff = triangle.vertices[2].depth - triangle.vertices[0].depth;

    float midPointHeightDiff = triangle.vertices[1].y - triangle.vertices[0].y;
    float midPointWidthDiff = (midPointHeightDiff / heightDiff) * widthDiff;
    float midPointDepthDiff = (midPointHeightDiff / heightDiff) * depthDiff;

    float midPointX = triangle.vertices[0].x + midPointWidthDiff;
    float midPointY = triangle.vertices[0].y + midPointHeightDiff;
    float midPointDepth = triangle.vertices[0].depth + midPointDepthDiff;

    TexturePoint a = triangle.vertices[0].texturePoint;
    TexturePoint b = triangle.vertices[2].texturePoint;

    float texHeight = b.y - a.y;
    float texWidth = b.x - a.x;

    float texMidPointX = a.x + (texWidth / (widthDiff == 0 ? 1 : widthDiff)) * midPointWidthDiff;
    float texMidPointY = a.y + (texHeight / (heightDiff == 0 ? 1 : heightDiff)) * midPointHeightDiff;

    CanvasPoint midPoint = CanvasPoint(midPointX, midPointY, midPointDepth);
    midPoint.texturePoint = TexturePoint(texMidPointX, texMidPointY);

    return midPoint;
}

CanvasTriangle convertToCanvasTriangle(ModelTriangle model, Camera camera, DrawingWindow window)
{
    CanvasTriangle triangle = CanvasTriangle();

    CanvasPoint a = convertToCanvasPoint(model.vertices[0], camera, window);
    CanvasPoint b = convertToCanvasPoint(model.vertices[1], camera, window);
    CanvasPoint c = convertToCanvasPoint(model.vertices[2], camera, window);

    return CanvasTriangle(a, b, c, model.colour);
}

RayTriangleIntersection getClosestIntersection(Camera camera, Ray ray, std::vector<ModelTriangle> triangles, int ignoreId)
{
    RayTriangleIntersection closestIntersection;

    // Go through each triangle and check if the ray is intersecting
    for (ModelTriangle triangle : triangles)
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
            closestIntersection.distance = distance;
            closestIntersection.intersectionPoint = ray.getStart() + distance * ray.getDirection();
            closestIntersection.intersectedTriangle = triangle;
            closestIntersection.hasHit = true;
        }
    }

    return closestIntersection;
}

std::string printVec(glm::vec3 vec) {
        std::stringstream ss;
        ss << "X: " << vec.x << ", Y: " << vec.y << ", Z: " << vec.z << std::endl;  
        return ss.str();
}

} // namespace utilities

CanvasPoint convertToCanvasPoint(glm::vec3 point, Camera camera, DrawingWindow window)
{
    // Model space -> Camera space
    // glm::vec3 updated = (point - camera.position) * camera.orientation;
    // glm::vec3 updated = (point - camera.position) * camera.orientation + camera.position;


    // Camera space -> Canvas Space
    float canvasX = window.scale * (camera.f *  point.x) / (-point.z) + window.width / 2.0f;
    float canvasY = window.scale * (camera.f * -point.y) / (-point.z) + window.height / 2.0f;
    float canvasD = 1.0f / point.z;

    return CanvasPoint(canvasX, canvasY, canvasD);
}