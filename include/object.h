#ifndef OBJECT_H
#define OBJECT_H

#include "material.h"
#include <iostream>
#include <cmath>
#include <string>
#include "matrix.h"
#include "texture.h"


class object{
protected:
    glm::vec3 loc, v, a, color;
    glm::vec3 rotate, rV, rA;
    std::string name;
    MATERIAL material;
    TEXTURE texture;
    unsigned int *textName = nullptr;
    float m;
public:
    object(glm::vec3 loc, glm::vec3 v, glm::vec3 a, float m);
    object();
    object(glm::vec3 loc);
    void update(float);
    void setV(glm::vec3);
    void addV(glm::vec3);
    void setA(glm::vec3);
    void addA(glm::vec3);
    void setRotateA(glm::vec3);
    void addRotateA(glm::vec3);
    void setRotateV(glm::vec3);
    void setLoc(glm::vec3);
    void setM(float);
    void setName(std::string);
    void setColor(glm::vec3);
    void getColor();
    void setMaterial(MATERIAL);
    void setTexture(TEXTURE, unsigned int *);
    glm::vec3 getV();
    glm::vec3 getA();
    glm::vec3 getLoc();
    glm::vec3 getRotate();
    glm::vec3 getRotateA();
    glm::vec3 getRotateV();
    float getM();
    std::string getName();
    virtual ~object();
    virtual void draw(MATERIAL, float, float, float);
    virtual void draw(MATERIAL);
    virtual void draw();
    virtual bool isCollide(object &);
    virtual glm::vec3 getBuoyancyCenter(float);
    virtual glm::vec3 getBuoyancyForce(float);
    virtual float getReflectedArea(float);
};

#endif