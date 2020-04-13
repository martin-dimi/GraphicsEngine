#include "Utilities.h"
#include <sstream>

CanvasPoint convertToCanvasPoint(glm::vec3& point, vec2& tex, Camera& camera, DrawingWindow& window);

namespace utilities
{
std::vector<float> interpolate(float from, float to, int steps)
{
    std::vector<float> numbers;

    float diff = to - from;
    float increment = steps == 1 ? diff : diff / (steps - 1);

    for (int step = 0; step < steps; step += 1)
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

void interpolate(std::vector<CanvasPoint>& points, CanvasPoint from, CanvasPoint to, int steps)
{
    if (steps <= 0)
        return;

    float xDiff = to.x - from.x;
    float yDiff = to.y - from.y;
    float dDiff = to.depth - from.depth;

    float xTexDiff = to.texturePoint.x - from.texturePoint.x;
    float yTexDiff = to.texturePoint.y - from.texturePoint.y;

    float divider = steps == 1 ? 1.0f : 1.0f / ( steps - 1 );

    float xStepSize = xDiff * divider;
    float yStepSize = yDiff * divider;
    float dStepSize = dDiff * divider;

    float xTexStepSize = xTexDiff * divider;
    float yTexStepSize = yTexDiff * divider;

    for (float step = 0; step < steps; step++)
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
}

CanvasPoint getTriangleMidPoint(CanvasTriangle triangle)
{

    float heightDiff = triangle.vertices[2].y - triangle.vertices[0].y;
    float widthDiff = triangle.vertices[2].x - triangle.vertices[0].x;
    float depthDiff = triangle.vertices[2].depth - triangle.vertices[0].depth;
    float depthInitDiff = triangle.vertices[2].initDepth - triangle.vertices[0].initDepth;

    float midPointHeightDiff = triangle.vertices[1].y - triangle.vertices[0].y;
    float ratio = (midPointHeightDiff / heightDiff);
    float midPointWidthDiff = ratio * widthDiff;
    float midPointDepthDiff = ratio * depthDiff;
    float midPointInitDepthDiff = ratio * depthInitDiff;

    float midPointX = triangle.vertices[0].x + midPointWidthDiff;
    float midPointY = triangle.vertices[0].y + midPointHeightDiff;
    float midPointDepth = triangle.vertices[0].depth + midPointDepthDiff;
    float midPointInitDepth = triangle.vertices[0].initDepth + midPointInitDepthDiff;

    TexturePoint& a = triangle.vertices[0].texturePoint;
    TexturePoint& b = triangle.vertices[2].texturePoint;

    float texWidth  = b.x - a.x;
    float texHeight = b.y - a.y;

    float texMidPointX = a.x + texWidth * ratio;
    float texMidPointY = a.y + texHeight * ratio;

    CanvasPoint midPoint = CanvasPoint(midPointX, midPointY, midPointDepth);
    midPoint.texturePoint = TexturePoint(texMidPointX, texMidPointY);
    midPoint.initDepth = midPointInitDepth;

    return midPoint;
}

CanvasTriangle convertToCanvasTriangle(ModelTriangle& model, Camera& camera, DrawingWindow& window)
{
    CanvasPoint a = convertToCanvasPoint(model.vertices[0], model.texture[0], camera, window);
    CanvasPoint b = convertToCanvasPoint(model.vertices[1], model.texture[1], camera, window);
    CanvasPoint c = convertToCanvasPoint(model.vertices[2], model.texture[2], camera, window);

    return CanvasTriangle(a, b, c, model.colour);
}

CanvasPoint convertToCanvasPoint(glm::vec3& point, Camera& camera, DrawingWindow& window)
{
    // float z = point.z == 0 ? 1.0f : 1.0f / (-point.z);
    vec3 p = (point - camera.position) * camera.orientation;
    float canvasX = window.scale * (camera.f *  p.x) / (-p.z) + window.halfWidth;
    float canvasY = window.scale * (camera.f * -p.y) / (-p.z) + window.halfHeight;

    return CanvasPoint(canvasX, canvasY, 0);
}

std::string printVec(glm::vec3 vec) {
        std::stringstream ss;
        ss << "X: " << vec.x << ", Y: " << vec.y << ", Z: " << vec.z << std::endl;  
        return ss.str();
}

} // namespace utilities

CanvasPoint convertToCanvasPoint(glm::vec3& point, vec2& tex, Camera& camera, DrawingWindow& window)
{
    // Model space -> Camera space
    // glm::vec3 updated = (point - camera.position) * camera.orientation + camera.position;

    // Camera space -> Canvas Space
    float canvasX = window.scale * (camera.f *  point.x) / (-point.z) + window.halfWidth;
    float canvasY = window.scale * (camera.f * -point.y) / (-point.z) + window.halfHeight;
    float canvasD = 1.0f / point.z;

    CanvasPoint p = CanvasPoint(canvasX, canvasY, canvasD);
    TexturePoint tp = TexturePoint(tex.x * (-canvasD), tex.y * (-canvasD));
    p.initDepth = point.z;
    p.texturePoint = tp;

    return p;
}