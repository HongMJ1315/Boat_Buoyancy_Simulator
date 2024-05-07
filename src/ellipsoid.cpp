#include "ellipsoid.h"
#include "cube.h"
#include "material.h"
#include "GLinclude.h"
#include "ocean.h"

ellipsoid::ellipsoid(glm::vec3 loc, float l, float w, float h) :object(loc){
    this->l = l;
    this->w = w;
    this->h = h;
}

void ellipsoid::draw(){
    if(textName != nullptr){
        SetTexture(texture, textName);
    }
    else
        SetMaterial(material, color.x, color.y, color.z);
    glPushMatrix();
    glTranslatef(object::getLoc().x, object::getLoc().y, object::getLoc().z);
    glRotatef(rotate.x, 1, 0, 0);
    glRotatef(rotate.y, 0, 1, 0);
    glRotatef(rotate.z, 0, 0, 1);
    glScalef(this->l, this->w, this->h);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
}

float ellipsoid::getL(){
    return l;
}

float ellipsoid::getW(){
    return w;
}

float ellipsoid::getH(){
    return h;
}

bool ellipsoid::isCollide(object &obj){
    return 0;
}

std::pair<float, float> ellipsoid::getY(float x, float z){
    //(x-loc.x)^2/l^2 + (y-loc.y)^2/w^2 + (z-loc.z)^2/h^2 = 1
    //y = loc.y + w * sqrt(1 - (x-loc.x)^2/l^2 - (z-loc.z)^2/h^2)
    float y1 = loc.y + w * sqrt(1 - (x - loc.x) * (x - loc.x) / (l * l) - (z - loc.z) * (z - loc.z) / (h * h));
    float y2 = loc.y - w * sqrt(1 - (x - loc.x) * (x - loc.x) / (l * l) - (z - loc.z) * (z - loc.z) / (h * h));
    return std::make_pair(y1, y2);
}

std::vector<ellipsoid::Cylinder> ellipsoid::decomposeEllipsoid(float t){
    std::vector<Cylinder> result;
    float dx = 0.1, dz = 0.1;
    for(float i = -l + loc.x; i <= l + loc.x; i += dx){
        for(float j = -h + loc.z; j <= h + loc.z; j += dz){
            Cylinder c;
            c.loc = glm::vec2(i, j);
            float waveHeight;
            getOcean(waveHeight, i, j, t);
            float inWaterY = std::min(waveHeight, getY(i, j).first);
            c.m = (inWaterY - getY(i, j).second) * dx * dz;
            c.h = std::max(inWaterY - getY(i, j).second, 0.0f);
            if(c.m > 0)
                result.push_back(c);
        }
    }
    return result;
}

glm::vec3 ellipsoid::getBuoyancyCenter(float t){
    std::vector<Cylinder> decomposed = decomposeEllipsoid(t);
    float totalM = 0;
    glm::vec3 result = glm::vec3(0, 0, 0);
    for(auto &c : decomposed){
        totalM += c.m;
        // std::cout << c.m << std::endl;
    }
    for(auto &c : decomposed){
        result += c.m / totalM * glm::vec3(c.loc.x, getY(c.loc.x, c.loc.y).second + c.h / 2.0, c.loc.y);
    }
    return result;
}

glm::vec3 ellipsoid::getBuoyancyForce(float t){
    std::vector<Cylinder> decomposed = decomposeEllipsoid(t);
    float totalM = 0;
    for(auto &c : decomposed){
        totalM += c.m;
    }
    return glm::vec3(0, totalM, 0);
}