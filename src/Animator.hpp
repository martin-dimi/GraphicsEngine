#pragma once

#include <glm/glm.hpp>
#include <DrawingWindow.h>
#include "model/Camera.hpp"
#include "model/World.hpp"
#include "model/Sphere.h"
#include "Colour.h"
#include "draw/Drawer.hpp"
#include "external/PPMImage.h"
#include "external/OBJFile.h"
#include "Utilities.h"
#include <vector>

using namespace std;
using namespace glm;

class Animator {

    public:
        bool isRunning;
        bool isUpdating;
        bool isRotating;

    private:
        World &world;
        int framerate;

        vector<vec3> cameraTransitions;
        vector<Sphere> sphereTransitions;
        int frame;
        int rotateFrame;

    public:
        Animator(World &world, int framerate) : world(world) {
            this->framerate = framerate;
            this->frame = 0;
            this->rotateFrame = 0;
            this->isRunning = false;
        }

        void addCameraTransition(vec3 position, float duration) {
            isUpdating = true;
            int steps = duration * framerate;
            auto cameraPosition = cameraTransitions.size() == 0 ? world.camera.position : cameraTransitions.back();

            auto newTransitions = utilities::interpolate(cameraPosition, position, steps);
            cameraTransitions.insert(cameraTransitions.end(), newTransitions.begin(), newTransitions.end());
        }

        void addSphereTransition(Sphere &from, Sphere &to, float duration) {
            isUpdating = true;
            int steps = duration * framerate;
            vector<Sphere> spheres;
            utilities::interpolate(spheres, from, to, steps);

            sphereTransitions.insert(sphereTransitions.end(), spheres.begin(), spheres.end());
        }

        void resetAnimation() {
            frame = 0;
            rotateFrame = 0;
            isRotating = true;
            isUpdating = true;
            sphereTransitions.clear();
        }

        void rotate(int duration) {


            float angle = 360.0f / (framerate * duration);

            if(rotateFrame >= 360.0/angle) {
                isRotating = false;
                return;
            }

            world.camera.rotate(glm::vec3(0.0f, 0.0f, 0.0f), angle);
            world.camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
            rotateFrame += 1;
        }

        void update() {

            if(sphereTransitions.size() > 0) 
            {
                if(frame == sphereTransitions.size()) {
                    isUpdating = false;
                    return;
                }

                world.clearMesh();
                world.addMesh(sphereTransitions[frame].mesh);
            }

            if(cameraTransitions.size() > 0) 
            {
                world.camera.position = cameraTransitions[frame];
                world.camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

                if(frame == cameraTransitions.size()) {
                    isUpdating = false;
                    resetAnimation();
                }
            }

            frame += 1;
        }

        bool isActive() {
            return isRunning && (isUpdating || isRotating);
        }
};