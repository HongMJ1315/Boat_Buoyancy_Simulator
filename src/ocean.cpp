#include "ocean.h"
#include <cmath>

float waveHeight(float x, float z, float t, float h, float lambda, float speed, float degree){
    float result = 0.0f;
    result = h * sin(2 * PI / lambda * (x * cos(degree) + z * sin(degree)) - speed * t);
    return result;
}

void getOcean(object *obj, float t){
    float ox = obj->getLoc().x;
    float oz = obj->getLoc().z;
    float h = waveHeight(ox, oz, t, WAVE_HEIGHT, WAVE_LAMBDA, WAVE_SPEED, WAVE_DEGREE);
    obj->setLoc(glm::vec3(ox, h, oz));
}

void getOcean(float &h, float x, float z, float t){
    h = waveHeight(x, z, t, WAVE_HEIGHT, WAVE_LAMBDA, WAVE_SPEED, WAVE_DEGREE);
}
