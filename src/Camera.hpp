#pragma once

class Camera {

public:
    int X, Y, Z;
    int f;

    Camera(int X, int Y, int Z, int f)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
        this->f = f;
    }

};