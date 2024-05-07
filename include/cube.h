#ifndef CUBE_H
#define CUBE_H

#include "object.h"
#include <vector>   

class cube : public object{
private:
    glm::vec3 rotatePoints[8] = {};
    float l, w, h;
    struct Cylinder{
        float h;
        float m;
        glm::vec2 loc; //(x, z)
    };
    std::vector<Cylinder> decomposeCube(float);
    void updateRotatePoints();
public:
    cube(glm::vec3, float, float, float);
    void draw();
    bool isCollide(object &);
    float getL();
    float getW();
    float getH();
    std::pair<float, float> getY(float, float);
    glm::vec3 getBuoyancyCenter(float);
    glm::vec3 getBuoyancyForce(float);
    // float getReflectedArea(float);
};

#endif