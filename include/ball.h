#ifndef BALL_H
#define BALL_H

#include "object.h"

class ball : public object{
private:
    float r;

public:
    ball(glm::vec3, float);
    void draw();
    bool isCollide(object &);
    float getR();
};

#endif