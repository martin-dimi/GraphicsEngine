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

using namespace std;
using namespace glm;

void draw();
void update();

const int WIDTH = 600;
const int HEIGHT = 400;

Camera camera = Camera(0.0f, 0.0f, 3.5f, 1.2f);
Light light = Light(0.0f, 0.9f, 0.0f, 35.0f);
// Light light = Light(-0.4f, 1.0f, 1.5f, 35.0f);
World world = World(camera, light);

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
unordered_map<string, int> state;

bool isSpinning = false;
bool showRaytracing = true;
int mode = 0;

int main(int argc, char *argv[])
{
    state["rotateAnimation"] = 0;
    state["displayMode"] = 0;

    // Sphere sphere = Sphere(15, 15, vec3(0.0f, 0.0f, 0.0f), 1, Colour(230, 70, 160));
    Sphere sphere = Sphere(12, 12, vec3(0.31f, -0.09f, 0.4f), 0.3f, Colour(180, 70, 160));
    world.addMesh(sphere.mesh);
    // world.addMesh(OBJFile("assets/Panter/panter2.obj", 1.0f));
    world.addMesh(OBJFile("assets/CornellBox/cornell-box.obj", 1.0f));
    // world.addMesh(OBJFile("assets/HackSpaceLogo/logo.obj", 1.0f, vec3(0.0f, 0.0f, -0.85f)));
    // world.addMesh(OBJFile("assets/Checkerboard/checkerboard.obj", 1.0f, vec3(0.0f, -0.4f, 0.0f)));


    EventHandler handler = EventHandler(window, world, state);

    while (true)
    {
        // We MUST poll for events - otherwise the window will freeze !
        handler.listenForEvents();
        update();
        draw();

        // Need to render the frame at the end, or nothing actually gets shown on the screen !
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
    // Function for performing animation (shifting artifacts or moving the camera)
    if (state["rotateAnimation"] == 0)
        return;

    // world.camera.translate(glm::vec3(1.0f, 0.0f, 0.0f), 0.3f);
    world.camera.rotate(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    world.camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    world.transformMeshToCameraSpace();
}
