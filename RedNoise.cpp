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

const int WIDTH = 600;
const int HEIGHT = 400;

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
Camera camera = Camera(0, 0, 100, HEIGHT/2);
OBJFile model = OBJFile("cornell-box/cornell-box.mtl", "cornell-box/cornell-box.obj", 30);

int main(int argc, char* argv[])
{
  SDL_Event event;

  while(true)
  {
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event);
    update();
    draw();
    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
}

void draw()
{
  window.clearPixels();

  // drawRedError(window);
  // drawGrayGradient(window);
  // drawColorGradient(window);

  // drawLine( CanvasPoint(15, 30), CanvasPoint(280, 212), Colour(255, 0, 0), window);


  loadModel(model, camera, window);
  drawLine(CanvasPoint(0, HEIGHT/2), CanvasPoint(WIDTH-1, HEIGHT/2), Colour(255, 255, 0), window);
  drawLine(CanvasPoint(WIDTH/2, 0), CanvasPoint(WIDTH/2, HEIGHT-1), Colour(255, 255, 0), window);
}

void update()
{
  // Function for performing animation (shifting artifacts or moving the camera)
}

void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT) camera.X -= 5;
    else if(event.key.keysym.sym == SDLK_RIGHT) camera.X += 5;
    else if(event.key.keysym.sym == SDLK_UP) camera.Y += 5;
    else if(event.key.keysym.sym == SDLK_DOWN) camera.Y -= 5;
    else if(event.key.keysym.sym == SDLK_l) camera.Z += 5;
    else if(event.key.keysym.sym == SDLK_k) camera.Z -= 5;

    else if(event.key.keysym.sym == SDLK_c) clear();
    else if(event.key.keysym.sym == SDLK_u) randomTrianlgeOutline();
    else if(event.key.keysym.sym == SDLK_f) randomTrianlgeFilled();
    else if(event.key.keysym.sym == SDLK_i) displayImage();
    else if(event.key.keysym.sym == SDLK_t) paintTexturedTriangle();
    else if(event.key.keysym.sym == SDLK_m) displayModel();

  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
}

void clear() {
  window.clearPixels();
}

void randomTrianlgeOutline() {
  CanvasPoint p1 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);
  CanvasPoint p2 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);
  CanvasPoint p3 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);

  Colour c = Colour(rand()%255, rand()%255, rand()%255);

  drawTriangleOutline(CanvasTriangle(p1, p2, p3), c, window);
}

void randomTrianlgeFilled() {
  CanvasPoint p1 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);
  CanvasPoint p2 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);
  CanvasPoint p3 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);

  // CanvasPoint p1 = CanvasPoint(125, 218);
  // CanvasPoint p2 = CanvasPoint(191, 152);
  // CanvasPoint p3 = CanvasPoint(124, 152);

  Colour c = Colour(rand()%255, rand()%255, rand()%255);

  drawTriangleFilled(CanvasTriangle(p1, p2, p3), c, window);
}
void paintTexturedTriangle() 
{

  PPMImage image = PPMImage("texture.ppm");

  CanvasPoint p1 = CanvasPoint(160, 10);
  CanvasPoint p2 = CanvasPoint(300, 230);
  CanvasPoint p3 = CanvasPoint(10, 150);

  p1.texturePoint = TexturePoint(195, 5);
  p2.texturePoint = TexturePoint(395, 380);
  p3.texturePoint = TexturePoint(65, 330);

  drawTriangleImageFilled(CanvasTriangle(p1, p2, p3), image, window);
}

void displayImage() 
{
  PPMImage image = PPMImage("texture.ppm");
  loadImage(image, window);
}

void displayModel()
{
  loadModel(model, camera, window);

}

