#pragma once

#include <glm/glm.hpp>

class Light {

    public:
        glm::vec3 location;
        float intensity;

        Light()
        {
            
        }

        Light(float X, float Y, float Z, float intensity)
        {
            this->location.x = X;
            this->location.y = Y;
            this->location.z = Z;
            this->intensity = intensity;
        }
        
};