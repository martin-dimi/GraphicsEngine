#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include "ModelTriangle.h"
#include "Utilities.h"

using namespace glm;
using namespace std;

class Sphere 
{
    private:
    int latDivisions;
    int longDivisions;
    vec3 position;
    float scale;
    Colour colour;
    vec3 max;
    vec3 min;

    vector<vec3> vertices;

    void generateVertices()
    {
        const vec3 base = vec3(0.0f, 1.0f, 0.0f);
        const float latAngle = M_PI / latDivisions;
        const float longAngle = 2.0f * M_PI / longDivisions;

        for(int lat = 1; lat < latDivisions; lat++)
        {
            const auto latBase = base * getRotationMatrixZ(latAngle * lat);

            for(int lon=0; lon < longDivisions; lon++)
            {
                auto vertex = latBase * getRotationMatrixY(longAngle * lon);
                vertex.z *= -1;

                if(vertex.x > this->max.x) this->max.x = vertex.x;
                if(vertex.y > this->max.y) this->max.y = vertex.y;
                if(vertex.z > this->max.z) this->max.z = vertex.z;

                if(vertex.x < this->min.x) this->min.x = vertex.x;
                if(vertex.y < this->min.y) this->min.y = vertex.y;
                if(vertex.z < this->min.z) this->min.z = vertex.z;

                vertices.push_back(vertex);
            }
        }

        // Caps
        vertices.push_back(base);
        vertices.push_back(-base);
    }

    void normaliseVertices() 
    {
        float xDiff = max.x - min.x;
        float yDiff = max.y - min.y;
        float zDiff = max.z - min.z;

        float scale = std::min(
            std::min(2.0f / xDiff, 2.0f / yDiff),
            2.0f / zDiff
        );

        for(int i=0; i < this->vertices.size(); i++)
        {
            glm::vec3 v = this->vertices[i];

            v.x = (v.x-0.5f*(min.x + max.x)) * scale * this->scale;
            v.y = (v.y-0.5f*(min.y + max.y)) * scale * this->scale;
            v.z = (v.z-0.5f*(min.z + max.z)) * scale * this->scale;

            this->vertices[i] = v + this->position;
        }
    }

    void generateMesh()
    {
        const auto getIndex = [this](int lat, int lon) { return lat*longDivisions + lon; };

        for(int lat = 0; lat < latDivisions-2; lat++)
        {
            for(int lon=0; lon < longDivisions-1; lon++)
            {
                mesh.push_back(getTriangle( getIndex(lat, lon), getIndex(lat+1, lon), getIndex(lat, lon+1)));
                mesh.push_back(getTriangle( getIndex(lat, lon+1), getIndex(lat+1, lon), getIndex(lat+1, lon+1)));
            }

            // wrap
            mesh.push_back(getTriangle( getIndex(lat, longDivisions-1), getIndex(lat+1, longDivisions-1), getIndex(lat, 0)));
            mesh.push_back(getTriangle( getIndex(lat, 0), getIndex(lat+1, longDivisions-1), getIndex(lat+1, 0)));
        }

        // cap
        for(int lon=0; lon < longDivisions-1; lon++)
        {
            // north
            mesh.push_back(getTriangle(vertices.size()-2, getIndex(0, lon), getIndex(0, lon+1)));

            // south
            mesh.push_back(getTriangle( getIndex(latDivisions-2, lon +1), getIndex(latDivisions-2, lon),vertices.size()-1));
        }

        //wrap
        mesh.push_back(getTriangle(vertices.size()-2, getIndex(0, longDivisions-1), getIndex(0, 0)));
        mesh.push_back(getTriangle( getIndex(latDivisions - 2, 0), getIndex(latDivisions - 2, longDivisions - 1),vertices.size()-1));
    }

    ModelTriangle getTriangle(int i1, int i2, int i3)
    {
        ModelTriangle t = ModelTriangle( vertices[ i3 ], vertices[ i2 ], vertices[ i1 ], this->colour );
        t.normals[0] = normalize(vertices[ i3 ]);
        t.normals[1] = normalize(vertices[ i2 ]);
        t.normals[2] = normalize(vertices[ i1 ]);
        t.hasNormals = true;
        return t;
    }

    mat3 getRotationMatrixY(float angle)
    {
        return  mat3(vec3(cos(angle), 0.0, sin(angle)),
                    vec3(0.0,        1.0,        0.0),
                    vec3(-sin(angle), 0.0, cos(angle)));
    }

    mat3 getRotationMatrixZ(float angle)
    {
        return  mat3 (vec3(cos(angle), -sin(angle), 0.0f),
                      vec3(sin(angle), cos(angle), 0.0f),
                      vec3(0.0f, 0.0f, 1.0f));
    }

    public:
    vector<ModelTriangle> mesh;

    Sphere(int latDivisions, int longDivisions, vec3 position, float scale, Colour colour)
    {
        this->latDivisions = latDivisions;
        this->longDivisions = longDivisions;
        this->position = position;
        this->scale = scale;
        this->colour = colour;
        this->max = glm::vec3(-1000.0f, -1000.0f, -1000.0f);
        this->min = glm::vec3(1000.0f, 1000.0f, 1000.0f);

        generateVertices();
        normaliseVertices();
        generateMesh();
    }
};