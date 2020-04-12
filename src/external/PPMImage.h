#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Colour.h>
#include <DrawingWindow.h>
#include <TexturePoint.h>

class PPMImage {

    private:
        std::string path;
        int maxValue;
        std::vector<Colour> payload;

        bool readPPMFile(std::ifstream &imageFile);

    public:
        int width, height;
        
        PPMImage();
        PPMImage(std::string path);
        Colour getPixelValueAt(int index);
        Colour getPixelValueAt(int x, int y);
        Colour getPixelValueAt(float x, float y);
        Colour getPixelValueAt(TexturePoint point);
        Colour getPixelValueAt(glm::vec2 point);

        static void saveImage(std::string name, DrawingWindow &window);
};