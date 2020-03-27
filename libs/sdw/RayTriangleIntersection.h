#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "ModelTriangle.h"

class RayTriangleIntersection
{
public:
    glm::vec3 intersectionPoint;
    float distance;
    ModelTriangle intersectedTriangle;
    bool hasHit = false;

    RayTriangleIntersection()
    {
        distance = std::numeric_limits<float>::max();
    }

    RayTriangleIntersection(glm::vec3 point, float distance, ModelTriangle triangle)
    {
        this->intersectionPoint = point;
        this->distance = distance;
        this->intersectedTriangle = triangle;
    }
};

// std::ostream &operator<<(std::ostream &os, const RayTriangleIntersection &intersection)
// {
//     os << "Intersection is at " << intersection.intersectionPoint << " on triangle " << intersection.intersectedTriangle << " at a distance of " << intersection.distanceFromCamera << std::endl;
//     return os;
// }
