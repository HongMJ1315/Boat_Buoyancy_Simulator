#include "material.h"
#include "GLinclude.h"
#include "object.h"
#include <iostream>


object::object(glm::vec3 loc, glm::vec3 v, glm::vec3 a, float m){
    this->loc = loc;
    this->v = v;
    this->a = a;
    this->material = MATERIAL::M_NONE;
    this->texture = TEXTURE::T_NONE;
}

object::object(){
    this->loc = glm::vec3(0.0f, 0.0f, 0.0f);
    this->v = glm::vec3(0.0f, 0.0f, 0.0f);
    this->a = glm::vec3(0.0f, 0.0f, 0.0f);
    this->material = MATERIAL::M_NONE;
    this->texture = TEXTURE::T_NONE;
}

object::object(glm::vec3 loc){
    this->loc = loc;
    this->v = glm::vec3(0.0f, 0.0f, 0.0f);
    this->a = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotate = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rV = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rA = glm::vec3(0.0f, 0.0f, 0.0f);
    this->material = MATERIAL::M_NONE;
    this->texture = TEXTURE::T_NONE;
}

void object::draw(MATERIAL m, float r, float g, float b){
    std::cout << "object draw" << std::endl;
}

void object::draw(MATERIAL m){
    std::cout << "object draw" << std::endl;
}

glm::vec3 object::getLoc(){
    return this->loc;
}

bool object::isCollide(object &obj){
    std::cout << "object collide" << std::endl;
    return 0;
}

void object::update(float dt){
    this->v += this->a * dt;
    this->loc += this->v * dt;
    this->rV += this->rA * dt;
    this->rotate += this->rV * dt;
}

void object::setV(glm::vec3 v){
    this->v = v;
}

void object::setA(glm::vec3 a){
    this->a = a;
}

void object::setName(std::string name){
    this->name = name;
}

void object::setLoc(glm::vec3 loc){
    this->loc = loc;
}

void object::setM(float m){
    this->m = m;
}

glm::vec3 object::getV(){
    return this->v;
}

glm::vec3 object::getA(){
    return this->a;
}

std::string object::getName(){
    return this->name;
}

float object::getM(){
    return this->m;
}

object::~object(){
    std::cout << "object destruct" << std::endl;
}

void object::setColor(glm::vec3 color){
    this->color = color;
}

void object::getColor(){
    std::cout << "color: " << color.x << " " << color.y << " " << color.z << std::endl;
}

void object::draw(){
    std::cout << "object draw" << std::endl;
}

void object::setMaterial(MATERIAL m){
    this->material = m;
}

void object::setTexture(TEXTURE t, unsigned int *textName){
    this->texture = t;
    this->textName = textName;
}

void object::addA(glm::vec3 a){
    this->a += a;
}

void object::addV(glm::vec3 v){
    this->v += v;
}

void object::setRotateA(glm::vec3 rA){
    this->rA = rA;
}

void object::addRotateA(glm::vec3 rA){
    this->rA += rA;
}

void object::setRotateV(glm::vec3 rV){
    this->rV = rV;
}

glm::vec3 object::getRotate(){
    return this->rotate;
}

glm::vec3 object::getRotateA(){
    return this->rA;
}

glm::vec3 object::getRotateV(){
    return this->rV;
}

glm::vec3 object::getBuoyancyCenter(float waterLevel){
    return this->loc;
}

glm::vec3 object::getBuoyancyForce(float waterLevel){
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

float object::getReflectedArea(float waterLevel){
    return 0.0f;
}