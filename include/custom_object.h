#ifndef CUSTOM_OBJECT_H
#define CUSTOM_OBJECT_H

#include "object.h"
#include "GLinclude.h" // Assuming this header contains necessary OpenGL includes
#include <vector>

class CustomObject : public object{
private:
    GLuint vao, vbo;
    std::vector<glm::vec3> vertices; // Vertices data

public:
    CustomObject(glm::vec3 loc, glm::vec3 v, glm::vec3 a, float m);
    void initializeVertices(std::vector<glm::vec3>&); // Initialize vertices data
    void draw() override; // Override draw function for custom drawing
};

#endif