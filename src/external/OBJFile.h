#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#include "Colour.h"
#include "CanvasPoint.h"
#include "ModelTriangle.h"
#include "external/PPMImage.h"

using namespace std;

class OBJFile 
{
    private:
        string path;
        string objName;
        unordered_map<string, Colour> pallete;
        vector<glm::vec3> vertecies;
        vector<glm::vec2> textureVertecies;

        float scale;
        glm::vec3 max;
        glm::vec3 min;

        void read();
        void readMaterials(ifstream* file);
        void readVertices(ifstream* file);
        void readFaces(ifstream* file);

        void readPallet(string materialName);
        Colour readColour(string* words);
        void readFace(string* words, Colour colour);
        void readVertex(string* words);
        void readTextureVertex(string* words);

        int normaliseChannel(float c);
        void normaliseVertices();

    public:
        OBJFile();
        OBJFile(string pathObj);
        OBJFile(string pathObj, float scale);
        vector<ModelTriangle> faces;
        PPMImage texture;
};

