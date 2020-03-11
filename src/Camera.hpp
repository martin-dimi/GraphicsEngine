#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Camera {

public:
    float f;
    
    // 0 - right, 1 - up, 2 - forward
    mat3 orientation;
    vec3 position;

    Camera(float f): Camera(0.0f, 0.0f, 0.0f, f) 
    { 
        // Default constructor
    }

    Camera(float X, float Y, float Z, float f)
    {
        position = vec3(X, Y, Z);
        orientation = mat3(1.0f);
        this->f = f;
    }

    void lookAt(vec3 point)
    {
        vec3 forward = normalize(position - point);
        vec3 right   = normalize(cross(vec3(0.0f, 1.0f, 0.0f), forward));
        vec3 up      = normalize(cross(forward, right));

        orientation[0] = right;
        orientation[1] = up;
        orientation[2] = forward;
    }

    void translate(vec3 direction, float distance)
    {
        vec3 dir = normalize(orientation * direction);
        position = dir * distance + position;
    }

    void rotate(vec3 pivot, float angle) 
    {
        angle = radians(angle);
        mat3 rotationMatrix(vec3(cos(angle), 0.0, sin(angle)),
                            vec3(0.0,        1.0,        0.0),
                            vec3(-sin(angle), 0.0, cos(angle)));

        position = ((position - pivot) * rotationMatrix) + pivot;
    }

    void tilt(float angle) 
    {
        angle = radians(angle);
        mat3 rotationMatrix(vec3(1.0, 0.0, 0.0),
                            vec3(0.0, cos(angle), -sin(angle)),
                            vec3(0.0, sin(angle),  cos(angle)));

        orientation = orientation * rotationMatrix;
    }

    void pan(float angle) 
    {
        angle = radians(angle);

        mat3 rotationMatrix(vec3(cos(angle), 0.0, sin(angle)),
                            vec3(0.0,        1.0,        0.0),
                            vec3(-sin(angle), 0.0, cos(angle)));

        orientation = orientation * rotationMatrix;
    }
};