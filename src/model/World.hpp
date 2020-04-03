#pragma once

#include "ModelTriangle.h"
#include "model/Light.hpp"
#include "model/Camera.hpp"
#include "external/OBJFile.h"
#include "external/PPMImage.h"

#include <glm/glm.hpp>
#include <vector>

using namespace std;

class World 
{
    private:
        vector<ModelTriangle> mesh;
        vector<ModelTriangle> meshWithCameraPrespective;

        ModelTriangle transformToCameraSpace(ModelTriangle triangle)
        {
            triangle.vertices[0] = (triangle.vertices[0] - camera.position) * camera.orientation;
            triangle.vertices[1] = (triangle.vertices[1] - camera.position) * camera.orientation;
            triangle.vertices[2] = (triangle.vertices[2] - camera.position) * camera.orientation;

            return triangle;
        } 

    public:
        Camera& camera;
        Light& light;
        PPMImage texture;

        World(Camera& c, Light& l) : camera(c), light(l) { }

        void addMesh(OBJFile const &obj)
        {
            for(ModelTriangle triangle : obj.faces) {
                this->mesh.push_back(triangle);
                this->meshWithCameraPrespective.push_back(transformToCameraSpace(triangle));
            }
            this->texture = obj.texture;
        }
        
        void transformMeshToCameraSpace()
        {
            for(int i=0; i<mesh.size(); i++)
            {
                ModelTriangle& triangle = mesh[i];
                meshWithCameraPrespective[i] = transformToCameraSpace(triangle);
            }
        }

        vector<ModelTriangle>& getMesh()
        {
            return this->meshWithCameraPrespective;
        }
};


