#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Colour.h>

class PPMImage {

    private:
        std::string path;
        int maxValue;
        std::vector<Colour> payload;

        bool readPPMFile(std::ifstream &imageFile);

    public:
        int width, height;
        
        PPMImage(std::string path);
        Colour getPixelValueAt(int index);
        Colour getPixelValueAt(int x, int y);

        void saveImage(std::string name);

};