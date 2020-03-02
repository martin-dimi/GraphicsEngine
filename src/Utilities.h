#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <ModelTriangle.h>
#include "Camera.hpp"

using namespace glm;

namespace utilities 
{
    std::vector<float> interpolate(float from, float to, int steps);
    std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to);
    std::vector<vec3> interpolate(vec3 from, vec3 to, int steps);
    std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to, int steps);

    CanvasPoint getTriangleMidPoint(CanvasTriangle triangle);
    CanvasTriangle convertToCanvasTriangle(ModelTriangle point, Camera camera, int WIDTH, int HEIGHT);
}