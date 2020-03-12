#include <ModelTriangle.h>
#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include "DrawUtils.h"
#include "PPMImage.h"
#include "OBJFile.h"
#include "Camera.hpp"

using namespace std;
using namespace glm;

void draw();
void update();
void handleEvent(SDL_Event event);
void randomTrianlgeOutline();
void randomTrianlgeFilled();
void clear();
void displayImage();
void paintTexturedTriangle();
void displayModel();
void switchMode();

const int WIDTH = 600;
const int HEIGHT = 400;

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
Camera camera = Camera(0.0f, 0.0f, 5.0f, 1.0f);
OBJFile model = OBJFile("assets/cornell-box.mtl", "assets/cornell-box.obj", 1.0f);

bool isSpinning = false;
bool showRaytracing = true;
int mode = 0;

int main(int argc, char *argv[])
{
    SDL_Event event;

    while (true)
    {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(&event))
            handleEvent(event);

        update();
        draw();

        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
    }
}

void draw()
{

    if(mode == 0)
    {
        window.clearPixels();
        loadModel(model, camera, window, true);
    }

    

    // drawLine(CanvasPoint(0, HEIGHT/2), CanvasPoint(WIDTH-1, HEIGHT/2), Colour(255, 255, 0), window);
    // drawLine(CanvasPoint(WIDTH/2, 0), CanvasPoint(WIDTH/2, HEIGHT-1), Colour(255, 255, 0), window);
}

void update()
{
    // Function for performing animation (shifting artifacts or moving the camera)
    if (!isSpinning)
        return;
    camera.translate(glm::vec3(1.0f, 0.0f, 0.0f), 0.3f);
    camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
}

void handleEvent(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_LEFT)camera.translate(glm::vec3(-1, 0, 0), 0.1f);
        else if (event.key.keysym.sym == SDLK_RIGHT)    camera.translate(glm::vec3(1, 0, 0), 0.1f);
        else if (event.key.keysym.sym == SDLK_UP)   camera.translate(glm::vec3(0, 1, 0), 0.1f);
        else if (event.key.keysym.sym == SDLK_DOWN) camera.translate(glm::vec3(0, -1, 0), 0.1f);
        else if (event.key.keysym.sym == SDLK_l)    camera.translate(glm::vec3(0, 0, 1), 0.1f);
        else if (event.key.keysym.sym == SDLK_k)    camera.translate(glm::vec3(0, 0, -1), 0.1f);
        else if (event.key.keysym.sym == SDLK_a)    camera.tilt(3);
        else if (event.key.keysym.sym == SDLK_s)    camera.tilt(-3);
        else if (event.key.keysym.sym == SDLK_f)    camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        else if (event.key.keysym.sym == SDLK_c)    clear();
        else if (event.key.keysym.sym == SDLK_i)    displayImage();
        else if (event.key.keysym.sym == SDLK_t)    paintTexturedTriangle();
        else if (event.key.keysym.sym == SDLK_m)    switchMode();

        else if (event.key.keysym.sym == SDLK_SPACE)    isSpinning = !isSpinning;
    }

    else if (event.type == SDL_MOUSEBUTTONDOWN)
        cout << "MOUSE CLICKED" << endl;
}

void switchMode() 
{
    mode = (mode + 1) % 3;

    clear();
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
}

void clear()
{
    window.clearPixels();
}

void randomTrianlgeOutline()
{
    CanvasPoint p1 = CanvasPoint(rand() % WIDTH, rand() % HEIGHT);
    CanvasPoint p2 = CanvasPoint(rand() % WIDTH, rand() % HEIGHT);
    CanvasPoint p3 = CanvasPoint(rand() % WIDTH, rand() % HEIGHT);

    Colour c = Colour(rand() % 255, rand() % 255, rand() % 255);

    drawTriangle(CanvasTriangle(p1, p2, p3, c), false, window);
}

void randomTrianlgeFilled()
{
    CanvasPoint p1 = CanvasPoint(rand() % WIDTH, rand() % HEIGHT);
    CanvasPoint p2 = CanvasPoint(rand() % WIDTH, rand() % HEIGHT);
    CanvasPoint p3 = CanvasPoint(rand() % WIDTH, rand() % HEIGHT);

    Colour c = Colour(rand() % 255, rand() % 255, rand() % 255);

    drawTriangle(CanvasTriangle(p1, p2, p3, c), true, window);
}
void paintTexturedTriangle()
{

    PPMImage image = PPMImage("assets/texture.ppm");

    CanvasPoint p1 = CanvasPoint(160, 10);
    CanvasPoint p2 = CanvasPoint(300, 230);
    CanvasPoint p3 = CanvasPoint(10, 150);

    p1.texturePoint = TexturePoint(195, 5);
    p2.texturePoint = TexturePoint(395, 380);
    p3.texturePoint = TexturePoint(65, 330);

    drawTriangleTexture(CanvasTriangle(p1, p2, p3), image, window);
}

void displayImage()
{
    PPMImage image = PPMImage("assets/texture.ppm");
    loadImage(image, window);
}

void displayModel()
{
    clear();

    if(showRaytracing) {
        raytrace(model, camera, window);
         std::cout << "Changed to Raytracing" << std::endl;
    }
    else {
        loadModel(model, camera, window, true);
        std::cout << "Changed to Rasterizing" << std::endl;
    }
   
    showRaytracing = !showRaytracing;
}
