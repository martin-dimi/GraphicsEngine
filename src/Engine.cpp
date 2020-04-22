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

Camera camera = Camera(0.0f, 0.0f, 3.5f, 1.2f);
Light light = Light(0.0f, 0.9f, 0.0f, 50.0f);
World world = World(camera, light);
Animator animator = Animator(world, 30);

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
unordered_map<string, int> state;

bool isSpinning = false;
bool showRaytracing = true;

int main(int argc, char *argv[])
{
    state["rotateAnimation"] = 0;
    state["displayMode"] = 0;
    camera.lookAt(vec3(0.0f, 0.0f, 0.0f));
    animator.isRotating = true;

    // world.addMesh(OBJFile("assets/Panter/panter2.obj", 1.0f));
    world.addMesh(OBJFile("assets/CornellBox/cornell-box.obj", 1.0f));
    // world.addMesh(OBJFile("assets/HackSpaceLogo/logo.obj", 1.5f, vec3(0.0f, 0.0f, -1.275f)));
    // world.addMesh(OBJFile("assets/Checkerboard/checkerboard.obj", 1.0f, vec3(0.0f, -0.4f, 0.0f)));
    // world.addMesh(OBJFile("assets/Triangle/triangle.obj", 1.0f, vec3(0.0f, -0.2f, 0.0f)));

    EventHandler handler = EventHandler(window, world, animator, state);

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
}

void update()
{
    // Simply the rotate anim
    if (state["rotateAnimation"] == 0)
        return;

    world.camera.rotate(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    world.camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    world.transformMeshToCameraSpace();
}
