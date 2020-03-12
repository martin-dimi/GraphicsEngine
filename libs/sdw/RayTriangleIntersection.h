#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "ModelTriangle.h"

class RayTriangleIntersection
{
public:
    glm::vec3 intersectionPoint;
    float distanceFromCamera;
    ModelTriangle intersectedTriangle;
    bool hasHit = false;

    RayTriangleIntersection()
    {
        distanceFromCamera = std::numeric_limits<float>::max();
    }

    RayTriangleIntersection(glm::vec3 point, float distance, ModelTriangle triangle)
    {
        intersectionPoint = point;
        distanceFromCamera = distance;
        intersectedTriangle = triangle;
    }
};

// std::ostream &operator<<(std::ostream &os, const RayTriangleIntersection &intersection)
// {
//     os << "Intersection is at " << intersection.intersectionPoint << " on triangle " << intersection.intersectedTriangle << " at a distance of " << intersection.distanceFromCamera << std::endl;
//     return os;
// }
