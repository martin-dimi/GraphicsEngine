#include <ModelTriangle.h>
#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include "draw/Drawer.hpp"
#include "external/OBJFile.h"
#include "EventHandler.cpp"
#include "model/Camera.hpp"
#include "model/Light.hpp"
#include "Utilities.h"

using namespace std;
using namespace glm;

void draw();
void update();

const int WIDTH = 600;
const int HEIGHT = 400;

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
Camera camera        = Camera(0.0f, 0.0f, 3.0f, 1.0f);
OBJFile model        = OBJFile("assets/cornell-box.mtl", "assets/cornell-box.obj", 1.0f);
EventHandler handler = EventHandler(window, camera, model);

Light lightSource = Light(0.0f, 0.9f, 0.0f, 25.0f);
unordered_map<string, int> state;

bool isSpinning = false;
bool showRaytracing = true;
int mode = 0;

int main(int argc, char *argv[])
{
    model.lightSource = lightSource;
    model.transformToCameraSpace(camera);
    state["rotateAnimation"] = 0;
    state["displayMode"] = 0;

    while (true)
    {
        // We MUST poll for events - otherwise the window will freeze !
        handler.listenForEvents(&state);
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
        drawModelWireframe(model, camera, window);
    }

    // drawLine(CanvasPoint(0, HEIGHT/2), CanvasPoint(WIDTH-1, HEIGHT/2), Colour(255, 255, 0), window);
    // drawLine(CanvasPoint(WIDTH/2, 0), CanvasPoint(WIDTH/2, HEIGHT-1), Colour(255, 255, 0), window);
}

void update()
{
    // Function for performing animation (shifting artifacts or moving the camera)
    if (state["rotateAnimation"] == 0)
        return;

    camera.translate(glm::vec3(1.0f, 0.0f, 0.0f), 0.3f);
    camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    model.transformToCameraSpace(camera);
}
