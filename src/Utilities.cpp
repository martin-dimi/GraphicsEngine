#include "Utilities.h"

CanvasPoint convertToCanvasPoint(glm::vec3 vector, Camera camera);

namespace utilities
{
    std::vector<float> interpolate(float from, float to, int steps) {
        std::vector<float> numbers;

        if(steps - 1 == 0) return numbers; 

        float diff = to - from;
        float increment = diff / (steps - 1);

        for(float step = 0; step < steps; step += 1)
            numbers.push_back(from + step * increment);

        return numbers; 
    }

    std::vector<CanvasPoint> interpolate2d(CanvasPoint from, CanvasPoint to) 
    {
        std::vector<CanvasPoint> numbers;
        
        int xDiff = to.x - from.x;
        int yDiff = to.y - from.y;

        float steps = max(abs(xDiff), abs(yDiff));

        float xStepSize = xDiff / steps;
        float yStepSize = yDiff / steps;

        for(int step=0; step < steps; step++) {
            float x = from.x + step * xStepSize;
            float y = from.y + step * yStepSize;

            numbers.push_back(CanvasPoint(x, y));
        }

        return numbers;
    }

    std::vector<vec3> interpolate3d(glm::vec3 from, glm::vec3 to, int steps) {
        std::vector<vec3> numbers;
        vec3 diff = to - from;
        vec3 increment = diff / float(steps - 1);

        for(float step = 0; step <= steps; step += 1) {
            vec3 cur = from + step * increment;
            numbers.push_back(cur);
        }

        return numbers;
    }

    std::vector<CanvasPoint> interpolateCanvasPoint(CanvasPoint from, CanvasPoint to, int steps) {
        std::vector<CanvasPoint> points;

        if(steps <= 0) return points;

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

        for(float step = 0; step < steps; step += 1) {
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

        // float ratio = midPointHeightDiff / heightDiff - midPointHeightDiff;

        float texHeight = b.y - a.y;
        float texWidth  = b.x - a.x;

        float texMidPointX = a.x + (texWidth / widthDiff) * midPointWidthDiff;
        float texMidPointY = a.y + (texHeight / heightDiff) * midPointHeightDiff;

        CanvasPoint midPoint = CanvasPoint(midPointX, midPointY, midPointDepth);
        midPoint.texturePoint = TexturePoint(texMidPointX, texMidPointY);

        return midPoint;
    }

    // CanvasPoint getTriangleMidPoint(CanvasTriangle triangle)
    // {
    //     int steps = triangle.vertices[2].y - triangle.vertices[0].y + 1;
    //     int midStep = triangle.vertices[1].y - triangle.vertices[0].y + 1;

    //     std::vector<CanvasPoint> points = interpolateCanvasPoint(triangle.vertices[0], triangle.vertices[2], steps);

    //     return points[midStep];
    // }

    CanvasTriangle convertToCanvasTriangle(ModelTriangle model, Camera camera)
    {
        CanvasTriangle triangle = CanvasTriangle();

        CanvasPoint a = convertToCanvasPoint(model.vertices[0], camera);
        CanvasPoint b = convertToCanvasPoint(model.vertices[1], camera);
        CanvasPoint c = convertToCanvasPoint(model.vertices[2], camera);

        return CanvasTriangle(a, b, c, model.colour);
    }
    
}

CanvasPoint convertToCanvasPoint(glm::vec3 vector, Camera camera)
{
    // Model space -> Camera space
    float X = vector[0] - camera.X;
    float Y = vector[1] - camera.Y;
    float Z = vector[2] - camera.Z;

    // Camera space -> Canvas Space
    float canvasX = ( camera.f *  X ) / ( -Z ) + WIDTH/2;
    float canvasY = ( camera.f * -Y ) / ( -Z ) + HEIGHT/2;
    float canvasD = 1/Z;

    return CanvasPoint(canvasX, canvasY, canvasD);
}