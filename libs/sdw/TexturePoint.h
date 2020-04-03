#pragma once
#include <iostream>
#include <glm/glm.hpp>

class TexturePoint
{
  public:
    float x;
    float y;

    TexturePoint()
    {
    }

    TexturePoint(float xPos, float yPos)
    {
      x = xPos;
      y = yPos;
    }

    TexturePoint(glm::vec2 point)
    {
      x = point.x;
      y = point.y;
    }

    void print()
    {
    }
};

inline std::ostream& operator<<(std::ostream& os, const TexturePoint& point)
{
    os << "(" << point.x << ", " << point.y << ")" << std::endl;
    return os;
}
