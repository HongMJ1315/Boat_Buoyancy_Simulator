#ifndef ELLIPTSOID_H
#define ELLIPTSOID_H

#include "object.h"
#include <cmath>
#include <vector>

#define INF 10000000
class ellipsoid : public object{
private:
    float l, w, h;
    struct Cylinder{
        float m;
        float h;
        glm::vec2 loc; //(x, z)
    };
    std::vector<ellipsoid::Cylinder> decompose;
    // true: 在球內(含面上) false: 在球外
    bool checkIn(glm::vec3);
    glm::vec3 getABC(float p, float lv, float loc, float *whl, int index);


public:
    ellipsoid(glm::vec3, float, float, float);
    void decomposeEllipsoid(float);
    void draw();
    bool isCollide(object &);
    float getL();
    float getW();
    float getH();
    std::pair<float, float> getY(float, float);
    glm::vec3 getBuoyancyCenter(float);
    glm::vec3 getBuoyancyForce(float);
    float getReflectedArea(float);
};

#endif