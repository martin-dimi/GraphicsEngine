#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#include "Colour.h"
#include "CanvasPoint.h"
#include "ModelTriangle.h"
#include "model/Light.hpp"
#include "Camera.hpp"

using namespace std;

class OBJFile 
{

    private:
        string pathMtl;
        string pathObj;
        float scale;
        unordered_map<string, Colour> pallete;
        vector<glm::vec3> vertecies;
        glm::vec3 max;
        glm::vec3 min;

        void readPallet();
        void readTriangles();
        int normaliseChannel(float c);
        void normaliseVertices();
        void readVertex(string* words);
        Colour readColour(string* words);
        void readFace(string* words, Colour colour);

    public:
        OBJFile();
        OBJFile(string pathMtl, string pathObj);
        OBJFile(string pathMtl, string pathObj, float scale);
        vector<ModelTriangle> faces;
        vector<ModelTriangle> loadedFaces;
        Light lightSource;

        void transformToCameraSpace(Camera camera);

};

