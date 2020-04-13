#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "ModelTriangle.h"

class RayIntersection
{
public:
    glm::vec3 intersectionPoint;
    glm::vec3 intersectionNormal;
    ModelTriangle intersectedTriangle;
    float distance;
    bool hasHit = false;
    float intersectionBrightness;
    Colour intersectionColour;

    RayIntersection()
    {
        distance = std::numeric_limits<float>::max();
        intersectionBrightness = 1.0f;
    }

    RayIntersection(glm::vec3 point, float distance, ModelTriangle triangle)
    {
        this->intersectionPoint = point;
        this->distance = distance;
        this->intersectedTriangle = triangle;
        this->intersectionBrightness = 1.0f;
    }
};
