#include "Utilities.h"

CanvasPoint convertToCanvasPoint(glm::vec3 vector, Camera camera, int WIDTH, int HEIGHT);

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

    std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to) 
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

    std::vector<vec3> interpolate(glm::vec3 from, glm::vec3 to, int steps) {
        std::vector<vec3> numbers;
        vec3 diff = to - from;
        vec3 increment = steps == 1 ? diff : diff / float(steps - 1);

        for(float step = 0; step <= steps; step += 1) {
            vec3 cur = from + step * increment;
            numbers.push_back(cur);
        }

        return numbers;
    }

    std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to, int steps) {
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

        float texHeight = b.y - a.y;
        float texWidth  = b.x - a.x;

        float texMidPointX = a.x + (texWidth / (widthDiff == 0 ? 1 : widthDiff)) * midPointWidthDiff;
        float texMidPointY = a.y + (texHeight / (heightDiff == 0 ? 1 : heightDiff)) * midPointHeightDiff;

        CanvasPoint midPoint = CanvasPoint(midPointX, midPointY, midPointDepth);
        midPoint.texturePoint = TexturePoint(texMidPointX, texMidPointY);

        return midPoint;
    }

    CanvasTriangle convertToCanvasTriangle(ModelTriangle model, Camera camera, int WIDTH, int HEIGHT)
    {
        CanvasTriangle triangle = CanvasTriangle();

        CanvasPoint a = convertToCanvasPoint(model.vertices[0], camera, WIDTH, HEIGHT);
        CanvasPoint b = convertToCanvasPoint(model.vertices[1], camera, WIDTH, HEIGHT);
        CanvasPoint c = convertToCanvasPoint(model.vertices[2], camera, WIDTH, HEIGHT);

        return CanvasTriangle(a, b, c, model.colour);
    } 
}

CanvasPoint convertToCanvasPoint(glm::vec3 point, Camera camera, int WIDTH, int HEIGHT)
{
    // Model space -> Camera space
    glm::vec3 updated = (point - camera.position) * camera.orientation;

    // Camera space -> Canvas Space
    float canvasX = ( camera.f *  updated.x ) / ( -updated.z ) + WIDTH/2.0f;
    float canvasY = ( camera.f * -updated.y ) / ( -updated.z ) + HEIGHT/2.0f;
    float canvasD = 1.0f / updated.z;

    return CanvasPoint(canvasX, canvasY, canvasD);
}