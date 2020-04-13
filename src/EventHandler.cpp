#include <glm/glm.hpp>
#include <DrawingWindow.h>
#include "model/Camera.hpp"
#include "model/World.hpp"
#include "draw/Drawer.hpp"
#include "external/PPMImage.h"
#include "external/OBJFile.h"
#include "Utilities.h"

using namespace std;
using namespace glm;

class EventHandler {

    private:
        DrawingWindow &window;
        World &world;
        unordered_map<string, int> &state;

        SDL_Event event;
        

    public:
        EventHandler(DrawingWindow &window, World &world, unordered_map<string, int> &state) : window(window), world(world), state(state)
        {
            this->event = SDL_Event();
        }

        void listenForEvents()
        {
            if (window.pollForInputEvents(&event))
                handleEvent();
        }

        void handleEvent()
        {
            if (event.type == SDL_KEYDOWN)
            {
                // CAMERA MOVEMENT
                if (event.key.keysym.sym == SDLK_LEFT)          moveCamera(vec3(-1,  0,  0));
                else if (event.key.keysym.sym == SDLK_RIGHT)    moveCamera(vec3( 1,  0,  0));
                else if (event.key.keysym.sym == SDLK_UP)       moveCamera(vec3( 0,  1,  0));
                else if (event.key.keysym.sym == SDLK_DOWN)     moveCamera(vec3( 0, -1,  0));
                else if (event.key.keysym.sym == SDLK_w)        moveCamera(vec3( 0,  0, -1));
                else if (event.key.keysym.sym == SDLK_s)        moveCamera(vec3( 0,  0,  1));
                else if (event.key.keysym.sym == SDLK_r)        resetCamera();
                else if (event.key.keysym.sym == SDLK_d)        panCamera(1);
                else if (event.key.keysym.sym == SDLK_a)        panCamera(-1);
                else if (event.key.keysym.sym == SDLK_f)        
                {
                    world.camera.lookAt(vec3(0.0f, 0.0f, 0.0f));
                    world.transformMeshToCameraSpace();
                }

                // OTHER
                else if (event.key.keysym.sym == SDLK_c) window.clearPixels();
                else if (event.key.keysym.sym == SDLK_i) displayImage();
                else if (event.key.keysym.sym == SDLK_p) saveImage();
                else if (event.key.keysym.sym == SDLK_m) switchMode();

                else if (event.key.keysym.sym == SDLK_SPACE) triggerAnimation();
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN)
                cout << "MOUSE CLICKED" << endl;
        }

        void moveCamera(vec3 direction)
        {
            world.camera.translate(direction, 0.1f);
            world.transformMeshToCameraSpace();
        }

        void resetCamera()
        {
            world.camera.position.x = 0.0f;
            world.camera.position.y = 0.0f;
            world.camera.position.z = 3.0f;
            world.camera.orientation = glm::mat3(1.0f);
            world.transformMeshToCameraSpace();
        }

        void panCamera(int dir)
        {
            world.camera.pan(3 * dir);
            world.transformMeshToCameraSpace();
        }

        void displayImage()
        {
            PPMImage image = PPMImage("assets/texture.ppm");
            drawImage(image, window);
        }

        void saveImage()
        {
            PPMImage::saveImage("images/testImage", window);
        }

        void triggerAnimation()
        {
            state["rotateAnimation"] = (state["rotateAnimation"] + 1) % 2;
        }

        void switchMode() 
        {
            int mode = (state["displayMode"] + 1) % 3;    

            window.clearPixels();
            if(mode == 0) 
            {
                std::cout << "Switching to wireframe" << std::endl;
                drawModelWireframe(world, window);
            }
            else if(mode == 1) 
            {
                std::cout << "Switching to rasterizing" << std::endl;
                drawModel(world, window, false);
            }
            else if(mode == 2)
            {
                std::cout << "Switching to raytracing" << std::endl;
                drawModel(world, window, true);
            }

            state["displayMode"] = mode;
        }
};


