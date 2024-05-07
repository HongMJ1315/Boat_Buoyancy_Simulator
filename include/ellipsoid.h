#ifndef ELLIPTSOID_H
#define ELLIPTSOID_H

#include "object.h"
#include <vector>

class ellipsoid : public object{
private:
    float l, w, h;
    struct Cylinder{
        float m;
        float h;
        glm::vec2 loc; //(x, z)
    };
    std::vector<Cylinder> decomposeEllipsoid(float);

public:
    ellipsoid(glm::vec3, float, float, float);
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