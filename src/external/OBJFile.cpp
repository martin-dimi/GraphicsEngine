#include "OBJFile.h"
#include "Utils.h"

OBJFile::OBJFile() {}
OBJFile::OBJFile(string pathObj) :OBJFile::OBJFile(pathObj, 1.0f) {}
OBJFile::OBJFile(string pathObj, float scale)
{
    unsigned found = pathObj.find_last_of("/");
    this->path = pathObj.substr(0 , found) + "/";
    this->objName = pathObj.substr(found + 1);

    this->scale = scale;
    this->max = glm::vec3(-1000.0f, -1000.0f, -1000.0f);
    this->min = glm::vec3(1000.0f, 1000.0f, 1000.0f);

    read();

    cout << "Number of triangles: " << faces.size() << endl;
}

void OBJFile::read()
{
    ifstream file(path + objName);

    if (file) 
    {
        // Read Materials
        readMaterials(&file);

        // Reset file
        file.clear();
        file.seekg(0);

        // Read and normalize verticies
        readVertices(&file);
        normaliseVertices();

        // Reset file
        file.clear();
        file.seekg(0);

        // Read faces
        readFaces(&file);
    }
    else cout << "No such OBJ file: " << path + objName << endl;
}

void OBJFile::readMaterials(ifstream* file)
{
    string line;

    while(getline(*file, line))
    {
        if(line.size() == 0) continue;
        string* words = split(line, ' ');

        if(words[0] == "mtllib") readPallet(words[1]);
    }
}

void OBJFile::readVertices(ifstream* file)
{
    string line;

    while(getline(*file, line))
    {
        if(line.size() == 0) continue;
        string* words = split(line, ' ');

        if(words[0] == "v") readVertex(words);
        if(words[0] == "vt") readTextureVertex(words);
    }
}

void OBJFile::readFaces(ifstream* file)
{
    string line;
    Colour selectedColour = Colour(255, 255, 255);

    while(getline(*file, line))
    {
        if(line.size() == 0) continue;
        string* words = split(line, ' ');

        if(words[0] == "usemtl") selectedColour = readColour(words);
        else if(words[0] == "f") readFace(words, selectedColour);
    }
}

void OBJFile::readVertex(string* words) 
{
    float x = stof(words[1]);
    float y = stof(words[2]);
    float z = stof(words[3]);

    if(x > this->max.x) this->max.x = x;
    if(y > this->max.y) this->max.y = y;
    if(z > this->max.z) this->max.z = z;

    if(x < this->min.x) this->min.x = x;
    if(y < this->min.y) this->min.y = y;
    if(z < this->min.z) this->min.z = z;

    glm::vec3 v = glm::vec3(x, y, z);
    this->vertecies.push_back(v);
}

void OBJFile::readTextureVertex(string* words) 
{
    float x = stof(words[1]) * (texture.width-1);
    float y = stof(words[2]) * (texture.height-1);

    glm::vec2 v = glm::vec2(x, y);
    this->textureVertecies.push_back(v);
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
    triangle.id = faces.size();

    // Check if it has a texture coordinate
    if(words[1].substr(words[1].find('/')+1, words[1].length()).length() > 0)
    {
        int t1Index = stoi(words[1].substr(words[1].find('/')+1, words[1].length())) - 1;
        int t2Index = stoi(words[2].substr(words[2].find('/')+1, words[2].length())) - 1;
        int t3Index = stoi(words[3].substr(words[3].find('/')+1, words[3].length())) - 1;

        if(t2Index >= textureVertecies.size() || t1Index >= textureVertecies.size() || t3Index >= textureVertecies.size())
        {
            cout << "Accessing an undefined texture vertex" << endl;
            return;
        }

        triangle.texture[0] = textureVertecies[t1Index];
        triangle.texture[1] = textureVertecies[t2Index];
        triangle.texture[2] = textureVertecies[t3Index];
        triangle.isTextured = true;
    }

    faces.push_back(triangle);
}

int OBJFile::normaliseChannel(float c)
{
    return c * 255;
}

void OBJFile::normaliseVertices()
{

    float xDiff = max.x - min.x;
    float yDiff = max.y - min.y;
    float zDiff = max.z - min.z;

    float scale = std::min(
        std::min(2.0f / xDiff, 2.0f / yDiff),
        2.0f / zDiff
    );

    for(int i=0; i < this->vertecies.size(); i++)
    {
        glm::vec3 v = this->vertecies[i];

        v.x = (v.x-0.5f*(min.x + max.x)) * scale * this->scale;
        v.y = (v.y-0.5f*(min.y + max.y)) * scale * this->scale;
        v.z = (v.z-0.5f*(min.z + max.z)) * scale * this->scale;

        this->vertecies[i] = v;
    }
}

void OBJFile::readPallet(string materialName)
{
    string materialPath = path + materialName;
    ifstream materialFile(materialPath);

    if (materialFile) 
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

                Colour colour = Colour(name, redChannel, greenChannel, blueChannel);
                this->pallete[name] = colour;
            }

            else if (words[0] == "map_Kd")
            {
                this->texture = PPMImage(path + words[1]);
            }
        }
    }
    else cout << "No such material file: " << materialPath<< endl;
}
