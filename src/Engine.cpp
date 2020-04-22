#include <ModelTriangle.h>
#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include "draw/Drawer.hpp"
#include "external/OBJFile.h"
#include "model/Camera.hpp"
#include "model/Light.hpp"
#include "model/World.hpp"
#include "model/Sphere.h"
#include "EventHandler.cpp"
#include "Utilities.h"
#include "draw/Drawer.hpp"
#include "Animator.hpp"

using namespace std;
using namespace glm;

void draw();
void update();

const int WIDTH = 640;
const int HEIGHT = 480;

// Camera camera = Camera(0.0f, 0.0f, 3.5f, 1.2f);
// Camera camera = Camera(0.0f, 0.0f, 6.0f, 1.2f);
Camera camera = Camera(0.0f, 1.3f, 4.6f, 1.2f);
// Light light = Light(0.0f, 1.35f, 0.0f, 50.0f);
Light light = Light(-1.0f, 1.1f, 1.4f, 70.0f);
World world = World(camera, light);
Animator animator = Animator(world, 30);

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
unordered_map<string, int> state;

auto checkerboard = OBJFile("assets/Checkerboard/checkerboard.obj", 2.0f, vec3(0.0f, -1.0f, 0.0f));

bool isSpinning = false;
bool showRaytracing = true;
int mode = 0;
int frame = 0;

Sphere sphere1 = Sphere(6, 6, vec3(0.0f, 0.0f, 0.0f), 0.4f, Colour(0, 10, 255));
Sphere sphere2 = Sphere(6, 6, vec3(0.0f, 0.0f, 0.0f), 1.0f, Colour(255, 51, 51));
Sphere sphere3 = Sphere(10, 10, vec3(0.0f, 0.0f, 0.0f), 1.0f, Colour(255, 255, 51));
Sphere sphere4 = Sphere(12, 12, vec3(0.0f, 0.0f, 0.0f), 1.0f, Colour(0, 255, 255));
Sphere sphere5 = Sphere(14, 14, vec3(0.0f, 0.0f, 0.0f), 1.0f, Colour(255, 128, 0));

int main(int argc, char *argv[])
{
    state["rotateAnimation"] = 0;
    state["displayMode"] = 0;
    camera.lookAt(vec3(0.0f, 0.0f, 0.0f));
    animator.isRotating = true;

    // world.addMesh(OBJFile("assets/Panter/panter2.obj", 1.0f));
    world.addMesh(OBJFile("assets/CornellBox/cornell-box.obj", 1.5f));
    // world.addMesh(OBJFile("assets/HackSpaceLogo/logo.obj", 1.5f, vec3(0.0f, 0.0f, -1.275f)));
    // world.addMesh(OBJFile("assets/Checkerboard/checkerboard.obj", 1.0f, vec3(0.0f, -0.4f, 0.0f)));
    // world.addMesh(OBJFile("assets/Triangle/triangle.obj", 1.0f, vec3(0.0f, -0.2f, 0.0f)));

    EventHandler handler = EventHandler(window, world, animator, state);
    // animator.addCameraTransition(vec3(-1.5f, 1.1f, 3.2f), 4);
    // animator.addCameraTransition(vec3(-1.5f, 1.1f, 3.2f), 1);
    // animator.addCameraTransition(vec3(1.36f, -0.1f, 4.3f), 3);
    // world.addMesh(sphere1.mesh);
    // world.addMesh(checkerboard);
    // animator.addSphereTransition(sphere1, sphere2, 10);
    // animator.addSphereTransition(sphere2, sphere3, 5);

    // Render loop
    while (true)
    {
        handler.listenForEvents();
        update();
        draw();
        window.renderFrame();
    }
}

void draw()
{
    if(state["displayMode"] == 0)
    {
        window.clearPixels();
        drawModelWireframe(world, window);        
    } 
    
    else if(state["displayMode"] == 1)
    {
        window.clearPixels();
        drawModel(world, window, false);
    }

    else if(state["displayMode"] == 2)
    {
        window.clearPixels();
        drawModel(world, window, true);
    }
}

void update()
{

    if(animator.isActive()) {
        // state["displayMode"] = 1;
        int duration = mode == 0 ? 5 : 5;
        animator.update();
        animator.rotate(duration);
        if(animator.isUpdating) world.addMesh(checkerboard);
        world.transformMeshToCameraSpace(); 
        frame += 1;
        PPMImage::saveImage("images/sphere/" + to_string(frame), window);
    }

    if(!animator.isRotating && mode == 0){
        state["displayMode"] = 1;
        animator.resetAnimation();
        mode++;

        animator.addSphereTransition(sphere3, sphere4, 5);
    }

    if(!animator.isRotating && mode == 1){
        state["displayMode"] = 2;
        animator.resetAnimation();
        mode++;

        animator.addSphereTransition(sphere4, sphere5, 5);
    }
}
