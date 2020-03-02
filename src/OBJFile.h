#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#include "Colour.h"
#include "CanvasPoint.h"
#include "ModelTriangle.h"

using namespace std;

class OBJFile 
{

    private:
        string pathMtl;
        string pathObj;
        int scale;
        unordered_map<string, Colour> pallete;
        vector<glm::vec3> vertecies;

        void readPallet();
        void readTriangles();
        int normaliseChannel(float c);
        void readVertex(string* words);
        Colour readColour(string* words);
        void readFace(string* words, Colour colour);

    public:
        OBJFile(string pathMtl, string pathObj);
        OBJFile(string pathMtl, string pathObj, int scale);
        vector<ModelTriangle> faces;

};

