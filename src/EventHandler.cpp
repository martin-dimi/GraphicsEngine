#include <glm/glm.hpp>
#include <DrawingWindow.h>
#include "Camera.hpp"
#include "DrawUtils.h"
#include "external/PPMImage.h"
#include "external/OBJFile.h"
#include "Utilities.h"

using namespace std;
using namespace glm;

class EventHandler {

    private:
        DrawingWindow &window;
        Camera &camera;
        OBJFile &model;
        SDL_Event event;
        
        unordered_map<string, int> *state;

    public:
        EventHandler(DrawingWindow &window, Camera &camera, OBJFile &model) : window(window), camera(camera), model(model) 
        {
            this->event = SDL_Event();
        }

        void listenForEvents(unordered_map<string, int> *state)
        {
            this->state = state;
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
                else if (event.key.keysym.sym == SDLK_d)        panCamera(1);
                else if (event.key.keysym.sym == SDLK_a)        panCamera(-1);
                else if (event.key.keysym.sym == SDLK_f)        
                {
                    camera.lookAt(vec3(0.0f, 0.0f, 0.0f));
                    model.transformToCameraSpace(camera);
                }

                // OTHER
                else if (event.key.keysym.sym == SDLK_c) window.clearPixels();
                else if (event.key.keysym.sym == SDLK_i) displayImage();
                else if (event.key.keysym.sym == SDLK_m) switchMode();

                else if (event.key.keysym.sym == SDLK_SPACE) triggerAnimation();
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN)
                cout << "MOUSE CLICKED" << endl;
        }

        void moveCamera(vec3 direction)
        {
            camera.translate(direction, 0.1f);
            model.transformToCameraSpace(camera);
        }

        void panCamera(int dir)
        {
            camera.pan(3 * dir);
            model.transformToCameraSpace(camera);
        }

        void displayImage()
        {
            PPMImage image = PPMImage("assets/texture.ppm");
            loadImage(image, window);
        }

        void triggerAnimation()
        {
            (*state)["rotateAnimation"] = ((*state)["rotateAnimation"] + 1) % 2;
        }

        void switchMode() 
        {
            int mode = ((*state)["displayMode"] + 1) % 3;    

            window.clearPixels();
            if(mode == 0) 
            {
                std::cout << "Switching to wireframe" << std::endl;
                loadModel(model, camera, window, true);
            }
            else if(mode == 1) 
            {
                std::cout << "Switching to rasterizing" << std::endl;
                loadModel(model, camera, window, false);
            }
            else if(mode == 2)
            {
                std::cout << "Switching to raytracing" << std::endl;
                raytrace(model, camera, window);
            }

            (*state)["displayMode"] = mode;
        }
};


