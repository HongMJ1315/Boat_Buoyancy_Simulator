#ifndef OCEAN_H
#define OCEAN_H

#include <algorithm>
#include <vector>
#include "GLinclude.h"
#include "object.h"
#define WAVE_HEIGHT .2f
#define WAVE_LAMBDA 5.0f
#define WAVE_SPEED 2.0f
#define WAVE_DEGREE 1.0f

#define PI glm::pi<float>()
float waveHeight(float x, float z, float t, float h, float lambda, float speed, float degree);
void getOcean(object *obj, float t);
void getOcean(float &h, float x, float z, float t);


#endif