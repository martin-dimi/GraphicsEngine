#pragma once
#include <iostream>

class Colour
{
  public:
    std::string name;
    int red;
    int green;
    int blue;

    Colour()
    {
    }

    Colour(int r, int g, int b)
    {
      name = "";
      red = r;
      green = g;
      blue = b;
    }

    Colour(std::string n, int r, int g, int b)
    {
      name = n;
      red = r;
      green = g;
      blue = b;
    }

    Colour(uint32_t packedColour)
    {
      name = "";
      red = (packedColour & 0x00FF0000) >> 16;
      green = (packedColour & 0x0000FF00) >> 8;
      blue = (packedColour & 0x000000FF);
    }

    u_int32_t getPackedInt()
    {
      return (255<<24) + (red<<16) + (green<<8) + blue;
    }

    u_int32_t getPackedInt(float brightness)
    {
      int upadtedRed = red * brightness;
      int upadtedGreen = green * brightness;
      int upadtedBlue = blue * brightness;
      
      return (255<<24) + (upadtedRed<<16) + (upadtedGreen<<8) + upadtedBlue;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Colour& colour)
{
    os << colour.name << " [" << colour.red << ", " << colour.green << ", " << colour.blue << "]" << std::endl;
    return os;
}
