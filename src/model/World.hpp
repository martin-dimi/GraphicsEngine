#pragma once

#include "ModelTriangle.h"
#include "model/Light.hpp"
#include "model/Camera.hpp"
#include "external/OBJFile.h"
#include "external/PPMImage.h"
#include <glm/glm.hpp>
#include "Utilities.h"

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

            triangle.normals[0] = glm::normalize(triangle.normals[0] * camera.orientation);
            triangle.normals[1] = glm::normalize(triangle.normals[1] * camera.orientation);
            triangle.normals[2] = glm::normalize(triangle.normals[2] * camera.orientation);

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

        void addMesh(vector<ModelTriangle> const &mesh)
        {
            for(ModelTriangle triangle : mesh) {
                this->mesh.push_back(triangle);
                auto m = transformToCameraSpace(triangle);
                // std::cout << m;
                this->meshWithCameraPrespective.push_back(m);
            }
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


