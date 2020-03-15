#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Ray 
{
    private:
        vec3 startPosition;
        vec3 direction;

    public:
        Ray(){}

        Ray(vec3 startPosition)
        {
            this->startPosition = startPosition;
        }

        Ray(vec3 startPosition, vec3 direction)
        {
            this->startPosition = startPosition;
            this->direction = direction;
        }

        vec3 getStart()
        {
            return this->startPosition;
        }

        vec3 getDirection()
        {
            return this->direction;
        }
};