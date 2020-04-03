#include "DrawUtils.h"
#include "Utilities.h"
#include "ModelTriangle.h"

#include <cmath>
#include <vector>

void fillTriangle(CanvasPoint& A, CanvasPoint& B, CanvasPoint& C, Colour& colour, DrawingWindow& window, float *depthBuffer);
void fillTriangle(CanvasPoint& A, CanvasPoint& B, CanvasPoint& C, PPMImage& image, DrawingWindow& window, float *depthBuffer);

namespace drawUtilities
{
    void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow& window)
    {
        int xDiff = b.x - a.x;
        int yDiff = b.y - a.y;

        float steps = std::max(abs(xDiff), abs(yDiff));
        if (steps == 0)
            return;

        steps += 1;
        float xStepSize = xDiff / steps;
        float yStepSize = yDiff / steps;

        for (int step = 0; step < steps; step++)
        {
            float x = a.x + step * xStepSize;
            float y = a.y + step * yStepSize;

            window.setPixelColour(x, y, c.getPackedInt());
        }
    }

    void drawImage(PPMImage image, DrawingWindow window)
    {
        for (int y = 0; y < image.height; y++)
        {
            for (int x = 0; x < image.width; x++)
            {
                Colour pixel = image.getPixelValueAt(image.width * y + x);
                window.setPixelColour(x, y, pixel.getPackedInt());
            }
        }
    }

    void drawTriangleOutline(CanvasTriangle& triangle, DrawingWindow& window)
    {
        drawUtilities::drawLine(triangle.vertices[0], triangle.vertices[1], triangle.colour, window);
        drawUtilities::drawLine(triangle.vertices[1], triangle.vertices[2], triangle.colour, window);
        drawUtilities::drawLine(triangle.vertices[2], triangle.vertices[0], triangle.colour, window);
    }

    void drawTriangleFilled(CanvasTriangle& triangle, DrawingWindow& window, float *depthBuffer)
    {
        // Sort verticies
        std::sort(triangle.vertices, triangle.vertices + 3, [](CanvasPoint const &a, CanvasPoint const &b) -> bool { return a.y < b.y; });

        CanvasPoint midPoint = utilities::getTriangleMidPoint(triangle);

        fillTriangle(triangle.vertices[0], triangle.vertices[1], midPoint, triangle.colour, window, depthBuffer);
        fillTriangle(triangle.vertices[2], midPoint, triangle.vertices[1], triangle.colour, window, depthBuffer);
    }

    void drawTriangleTextured(CanvasTriangle& triangle, PPMImage& image, DrawingWindow& window, float *depthBuffer)
    {
        // Sort verticies
        std::sort(triangle.vertices, triangle.vertices + 3, [](CanvasPoint const &a, CanvasPoint const &b) -> bool { return a.y < b.y; });

        CanvasPoint midPoint = utilities::getTriangleMidPoint(triangle);

        fillTriangle(triangle.vertices[0], triangle.vertices[1], midPoint, image, window, depthBuffer);
        fillTriangle(triangle.vertices[2], midPoint, triangle.vertices[1], image, window, depthBuffer);
    }
}

void fillTriangle(CanvasPoint& A, CanvasPoint& B, CanvasPoint& C, Colour& colour, DrawingWindow& window, float *depthBuffer)
{
    int height = std::ceil(abs(A.y - B.y) + 1);

    std::vector<CanvasPoint> from;
    std::vector<CanvasPoint> to;
    utilities::interpolate(from, A, B, height);
    utilities::interpolate(to, A, C, height);

    for (int row = 0; row < height; row++)
    {
        bool reverseOrder = from[row].x > to[row].x;
        CanvasPoint fromPoint = reverseOrder ? to[row] : from[row];
        CanvasPoint toPoint = reverseOrder ? from[row] : to[row];

        int y = fromPoint.y;
        if (y < 0 || y >= window.height - 1)
            continue;

        int width = std::ceil(toPoint.x - fromPoint.x + 1);
        std::vector<CanvasPoint> line;
        utilities::interpolate(line, fromPoint, toPoint, width);

        for (int col = 0; col < width; col++)
        {
            CanvasPoint p = line[col];
            int x = p.x;

            if (x < 0 || x >= window.width - 1)
                continue;

            if (depthBuffer != NULL)
            {
                int idx = (y * window.width) + x;
                if (depthBuffer[idx] > p.depth)
                {
                    window.setPixelColour(x, y, colour.getPackedInt());
                    depthBuffer[idx] = p.depth;
                }
            }
            else
                window.setPixelColour(x, y, colour.getPackedInt());
        }
    }
}

void fillTriangle(CanvasPoint& A, CanvasPoint& B, CanvasPoint& C, PPMImage& image, DrawingWindow& window, float *depthBuffer)
{
    int height = std::ceil(abs(A.y - B.y) + 1);

    std::vector<CanvasPoint> from;
    std::vector<CanvasPoint> to;
    utilities::interpolate(from, A, B, height);
    utilities::interpolate(to, A, C, height);

    for (int row = 0; row < height; row++)
    {
        bool reverseOrder = from[row].x > to[row].x;
        CanvasPoint fromPoint = reverseOrder ? to[row] : from[row];
        CanvasPoint toPoint = reverseOrder ? from[row] : to[row];

        int y = fromPoint.y;
        if (y < 0 || y >= window.height - 1)
            continue;

        int width = std::ceil(toPoint.x - fromPoint.x + 1);
        std::vector<CanvasPoint> line;
        utilities::interpolate(line, fromPoint, toPoint, width);

        for (int col = 0; col < width; col++)
        {
            CanvasPoint p = line[col];
            int x = p.x;

            if (x < 0 || x >= window.width - 1)
                continue;

            Colour colour = image.getPixelValueAt(p.texturePoint);

            if (depthBuffer != NULL)
            {
                int idx = (y * window.width) + x;
                if (depthBuffer[idx] > p.depth)
                {
                    window.setPixelColour(x, y, colour.getPackedInt());
                    depthBuffer[idx] = p.depth;
                }
            }
            else
                window.setPixelColour(x, y, colour.getPackedInt());
        }
    }
}