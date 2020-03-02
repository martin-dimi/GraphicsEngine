#include "OBJFile.h"
#include "Utils.h"

OBJFile::OBJFile(string pathMtl, string pathObj) :OBJFile::OBJFile(pathMtl, pathObj, 1) {}

OBJFile::OBJFile(string pathMtl, string pathObj, int scale)
{
    this->pathMtl = pathMtl;
    this->pathObj = pathObj;
    this->scale = scale;

    readPallet();   
    readTriangles();

    cout << "Number of triangles: " << faces.size() << endl;
}

void OBJFile::readPallet()
{
    ifstream materialFile(pathMtl);

    if (materialFile.is_open()) 
    {
        string line;
        while(getline(materialFile, line))
        {
            if(line.size() == 0) continue;

            string* words = split(line, ' ');
            if(words[0] == "newmtl")
            {
                string name = words[1];
                getline(materialFile, line);
                string* definition = split(line, ' ');

                int redChannel = this->normaliseChannel(stof(definition[1]));
                int greenChannel = this->normaliseChannel(stof(definition[2]));
                int blueChannel = this->normaliseChannel(stof(definition[3]));

                Colour colour = Colour(name, redChannel, blueChannel, greenChannel);
                this->pallete[name] = colour;
            }
        }
    }
    else cout << "No such file: " << pathObj << endl;
}

void OBJFile::readTriangles()
{
    ifstream geometryFile(pathObj);

    if (geometryFile.is_open()) 
    {
        string line;

        // Read all the verticies
        while(getline(geometryFile, line))
        {
            if(line.size() == 0) continue;
            string* words = split(line, ' ');

            if(words[0] == "v") readVertex(words);
        }

        // Read all the faces
        geometryFile.clear();
        geometryFile.seekg(0);
        Colour selectedColour = Colour(255, 255, 255);
        while(getline(geometryFile, line))
        {
            if(line.size() == 0) continue;
            string* words = split(line, ' ');

            if(words[0] == "usemtl") selectedColour = readColour(words);
            else if(words[0] == "f") readFace(words, selectedColour);
        }
    }
    else cout << "No such file: " << pathObj << endl;
}

void OBJFile::readVertex(string* words) 
{
    float x = stof(words[1]) * scale;
    float y = stof(words[2]) * scale;
    float z = stof(words[3]) * scale;

    glm::vec3 v = glm::vec3(x, y, z);
    this->vertecies.push_back(v);
}

Colour OBJFile::readColour(string* words)
{
    string colourName = words[1];
    if(pallete.count(colourName) > 0)
        return pallete[colourName];

    // Colour not found, defaulting to white
    cout << "Accessing an undefined colour: " << colourName << endl;
    return Colour(255, 255, 255);
}

void OBJFile::readFace(string* words, Colour colour)
{
    int p1Index = stoi(words[1].substr(0, words[1].find('/'))) - 1;
    int p2Index = stoi(words[2].substr(0, words[2].find('/'))) - 1;
    int p3Index = stoi(words[3].substr(0, words[3].find('/'))) - 1;

    if(p2Index >= vertecies.size() || p1Index >= vertecies.size() || p1Index >= vertecies.size())
    {
        cout << "Accessing an undefined vertex" << endl;
        return;
    }

    glm::vec3 p1 = vertecies[p1Index];
    glm::vec3 p2 = vertecies[p2Index];
    glm::vec3 p3 = vertecies[p3Index];

    ModelTriangle triangle = ModelTriangle(p1, p2, p3, colour);
    faces.push_back(triangle);
}

int OBJFile::normaliseChannel(float c)
{
    return c * 255;
}
