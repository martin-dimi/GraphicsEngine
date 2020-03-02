#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Camera {

public:
    int X, Y, Z;
    int f;
    
    vec3 position;

    // 0 - right, 1 - up, 2 - forward
    mat3 orientation;

    Camera(int f): Camera(0, 0, 0, f) { 
        // Default constructor
    }

    Camera(int X, int Y, int Z, int f)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
        this->f = f;

        position = vec3(X, Y, Z);
        orientation = mat3(1.0);

        for(int i = 0; i < 3; i++) {
            std::cout << "X: " << orientation[i].x << ", Y: " << orientation[i].y << ", Z: " << orientation[i].z << std::endl;
        }
    }

    void lookAt(vec3 point)
    {
        vec3 forward = position - point;
        vec3 right   = cross(vec3(0, 1, 0), forward);
        vec3 up      = cross(forward, right);

        orientation[0] = normalize(right);
        orientation[1] = normalize(up);
        orientation[2] = normalize(forward);
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