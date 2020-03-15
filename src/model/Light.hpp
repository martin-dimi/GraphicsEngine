#pragma once

#include <glm/glm.hpp>

class Light {

    private:
        glm::vec3 location;
        float intensity;

    public:
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

        glm::vec3 getLocation()
        {
            return this->location;
        }

};