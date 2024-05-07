#ifndef MATERIAL_H
#define MATERIAL_H

#include "GLinclude.h"

enum MATERIAL{
    M_OCEAN, M_OBJECT, M_NONE
};

void SetMaterial(MATERIAL, float, float, float);

#endif
