#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <ModelTriangle.h>
#include <DrawingWindow.h>
#include "Colour.h"
#include "model/Camera.hpp"
#include "model/Ray.hpp"
#include "model/RayIntersection.hpp"
#include "model/Sphere.h"

using namespace glm;

namespace utilities
{
std::string printVec(glm::vec3 vec);

std::vector<float> interpolate(float from, float to, int steps);
std::vector<CanvasPoint> interpolate(CanvasPoint from, CanvasPoint to);
std::vector<vec3> interpolate(vec3 from, vec3 to, int steps);
std::vector<Colour> interpolate(Colour from, Colour to, int steps);
void interpolate(std::vector<Sphere>& spheres, Sphere from, Sphere to, int steps);
void interpolate(std::vector<CanvasPoint>& points, CanvasPoint from, CanvasPoint to, int steps);

CanvasPoint getTriangleMidPoint(CanvasTriangle triangle);
CanvasTriangle convertToCanvasTriangle(ModelTriangle& model, Camera& camera, DrawingWindow& window);
CanvasPoint convertToCanvasPoint(glm::vec3& point, Camera& camera, DrawingWindow& window);
} // namespace utilities