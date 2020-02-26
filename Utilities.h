#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <ModelTriangle.h>
#include "Camera.hpp"

using namespace glm;

extern const int HEIGHT;
extern const int WIDTH;

namespace utilities 
{
    std::vector<float> interpolate(float from, float to, int steps);
    std::vector<CanvasPoint> interpolate2d(CanvasPoint from, CanvasPoint to);
    std::vector<vec3> interpolate3d(vec3 from, vec3 to, int steps);
    std::vector<CanvasPoint> interpolateCanvasPoint(CanvasPoint from, CanvasPoint to, int steps);

    CanvasPoint getTriangleMidPoint(CanvasTriangle triangle);
    CanvasTriangle convertToCanvasTriangle(ModelTriangle point, Camera camera);
}