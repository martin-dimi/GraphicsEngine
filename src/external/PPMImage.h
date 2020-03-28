#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Colour.h>
#include <DrawingWindow.h>

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

        static void saveImage(std::string name, DrawingWindow &window);
};