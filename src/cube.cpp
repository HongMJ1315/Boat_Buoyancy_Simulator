#include "ball.h"
#include "cube.h"
#include "material.h"
#include "ocean.h"
#include "GLinclude.h"

float points[][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
int faces[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };

cube::cube(glm::vec3 loc, float l, float w, float h) :object(loc){
    this->l = l;
    this->w = w;
    this->h = h;
}


void cube::draw(){
    if(textName != nullptr){
        SetTexture(texture, textName);
    }
    else
        SetMaterial(material, color.x, color.y, color.z);

    glPushMatrix();
    glTranslatef(loc.x, loc.y, loc.z);
    glRotatef(rotate.x, 1, 0, 0);
    glRotatef(rotate.y, 0, 1, 0);
    glRotatef(rotate.z, 0, 0, 1);
    glScalef(l, w, h);
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[faces[i][0]]);
        glTexCoord2f(10.0, 0.0);
        glVertex3fv(points[faces[i][1]]);
        glTexCoord2f(10.0, 10.0);
        glVertex3fv(points[faces[i][2]]);
        glTexCoord2f(0.0, 10.0);
        glVertex3fv(points[faces[i][3]]);
        glEnd();
    }
    glPopMatrix();
}

bool cube::isCollide(object &obj){
    cube *c = dynamic_cast<cube *>(&obj);
    if(c != nullptr){
        glm::vec2 cx = glm::vec2(loc.x - l / 2.0f, loc.x + l / 2.0f);
        glm::vec2 cy = glm::vec2(loc.y - w / 2.0f, loc.y + w / 2.0f);
        glm::vec2 cz = glm::vec2(loc.z - h / 2.0f, loc.z + h / 2.0f);
        glm::vec2 cx2 = glm::vec2(c->getLoc().x - c->getL() / 2.0f, c->getLoc().x + c->getL() / 2.0f);
        glm::vec2 cy2 = glm::vec2(c->getLoc().y - c->getW() / 2.0f, c->getLoc().y + c->getW() / 2.0f);
        glm::vec2 cz2 = glm::vec2(c->getLoc().z - c->getH() / 2.0f, c->getLoc().z + c->getH() / 2.0f);
        if(((cx.x - cx2.x < ESP && cx2.x - cx.y < ESP) || (cx2.x - cx.x < ESP && cx.x - cx2.y < ESP)) &&
            ((cy.x - cy2.x < ESP && cy2.x - cy.y < ESP) || (cy2.x - cy.x < ESP && cy.x - cy2.y < ESP)) &&
            ((cz.x - cz2.x < ESP && cz2.x - cz.y < ESP) || (cz2.x - cz.x < ESP && cz.x - cz2.y < ESP))){
            return 1;
        }
        else return 0;
    }
    ball *b = dynamic_cast<ball *>(&obj);
    if(b != nullptr){
        glm::vec2 bx = glm::vec2(b->getLoc().x - b->getR(), b->getLoc().x + b->getR());
        glm::vec2 by = glm::vec2(b->getLoc().y - b->getR(), b->getLoc().y + b->getR());
        glm::vec2 bz = glm::vec2(b->getLoc().z - b->getR(), b->getLoc().z + b->getR());
        glm::vec2 cx = glm::vec2(loc.x - l / 2.0f, loc.x + l / 2.0f);
        glm::vec2 cy = glm::vec2(loc.y - w / 2.0f, loc.y + w / 2.0f);
        glm::vec2 cz = glm::vec2(loc.z - h / 2.0f, loc.z + h / 2.0f);
        if(((bx.x - cx.x < ESP && cx.x - bx.y < ESP) || (cx.x - bx.x < ESP && bx.x - cx.y < ESP)) &&
            ((by.x - cy.x < ESP && cy.x - by.y < ESP) || (cy.x - by.x < ESP && by.x - cy.y < ESP)) &&
            ((bz.x - cz.x < ESP && cz.x - bz.y < ESP) || (cz.x - bz.x < ESP && bz.x - cz.y < ESP))){
            return 1;
        }
        else return 0;
    }
    return false;
}

float cube::getL(){
    return l;
}

float cube::getW(){
    return w;
}

float cube::getH(){
    return h;
}

void cube::updateRotatePoints(){
    // 計算三個方向的旋轉矩陣
    glm::mat3 rotateMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1, 0, 0))) *
        glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0, 1, 0))) *
        glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0, 0, 1)));
    // 計算8個頂點旋轉後的座標
    // 原始座標
    glm::vec3 originPoints[8];
    for(int i = 0; i < 8; i++){
        originPoints[i] = glm::vec3(points[i][0] * l, points[i][1] * w, points[i][2] * h);
    }
    // 旋轉後座標
    for(int i = 0; i < 8; i++){
        rotatePoints[i] = rotateMatrix * originPoints[i];
    }
    // 加上位置
    for(int i = 0; i < 8; i++){
        rotatePoints[i] += loc;
    }
}

std::pair<float, float> cube::getY(float x, float z){
    updateRotatePoints();
    // 利用頂點計算6個面的方程式
    float a[6], b[6], c[6], d[6];
    for(int i = 0; i < 6; i++){
        glm::vec3 v1 = rotatePoints[faces[i][1]] - rotatePoints[faces[i][0]];
        glm::vec3 v2 = rotatePoints[faces[i][2]] - rotatePoints[faces[i][0]];
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
        a[i] = normal.x;
        b[i] = normal.y;
        c[i] = normal.z;
        d[i] = -glm::dot(normal, rotatePoints[faces[i][0]]);
    }
    // 計算(x, z)在6個面的y值
    float y[6];
    for(int i = 0; i < 6; i++){
        y[i] = (-a[i] * x - c[i] * z - d[i]) / b[i];
    }
    // 得到8個平面Y最大最小值
    float maxY = -1000000, minY = 1000000;
    for(int i = 0; i < 8; i++){
        maxY = std::max(maxY, rotatePoints[i].y);
        minY = std::min(minY, rotatePoints[i].y);
    }
    // 尋找y陣列且在maxY和minY之間的最大最小值
    float retMinY = 1000000, retMaxY = -1000000;
    for(int i = 0; i < 6; i++){
        if(y[i] >= minY && y[i] <= maxY){
            retMinY = std::min(retMinY, y[i]);
            retMaxY = std::max(retMaxY, y[i]);
        }
    }
    return std::make_pair(retMaxY, retMinY);
}

glm::vec3 cube::getBuoyancyCenter(float t){
    float totalM = 0;
    glm::vec3 result = glm::vec3(0, 0, 0);
    // 特判 全部在水下
    float maxY = -1000000;
    float waveHeight;
    getOcean(waveHeight, loc.x, loc.z, t);
    for(int i = 0; i < 8; i++){
        maxY = std::max(maxY, rotatePoints[i].y);
    }
    if(maxY < waveHeight){
        return loc;
    }

    for(auto &c : decompose){
        totalM += c.m;
    }
    for(auto &c : decompose){
        result += c.m / totalM * glm::vec3(c.loc.x, getY(c.loc.x, c.loc.y).second + c.h / 2.0, c.loc.y);
    }
    return result;
}

glm::vec3 cube::getBuoyancyForce(float t){
    // 特判 全部在水下
    float maxY = -1000000;
    float waveHeight;
    getOcean(waveHeight, loc.x, loc.z, t);
    for(int i = 0; i < 8; i++){
        maxY = std::max(maxY, rotatePoints[i].y);
    }
    if(maxY < waveHeight){
        return glm::vec3(0, w * l * h, 0);
    }
    float totalM = 0;
    for(auto &c : decompose){
        totalM += c.m;
    }
    return glm::vec3(0, totalM, 0);
}

void cube::decomposeCube(float t){
    updateRotatePoints();
    decompose.clear();
    float minX = 1000000, maxX = -1000000, minZ = 1000000, maxZ = -1000000;
    for(int i = 0; i < 8; i++){
        minX = std::min(minX, rotatePoints[i].x);
        maxX = std::max(maxX, rotatePoints[i].x);
        minZ = std::min(minZ, rotatePoints[i].z);
        maxZ = std::max(maxZ, rotatePoints[i].z);
    }
    float dx = 0.1, dy = 0.1;
    for(float i = minX; i < maxX; i += dx){
        for(float j = minZ; j < maxZ; j += dy){
            Cylinder c;
            c.loc = glm::vec2(i, j);
            float waveHeight;
            getOcean(waveHeight, i, j, t);
            float inWaterY = std::min(waveHeight, getY(i, j).first);
            c.m = (inWaterY - getY(i, j).second) * dx * dy;
            c.h = std::max(inWaterY - getY(i, j).second, 0.0f);
            if(c.m > 0)
                decompose.push_back(c);
        }
    }
}

float cube::getReflectedArea(float t){
    updateRotatePoints();
    // 特判 全部在水下
    float maxY = -1000000;
    float waveHeight;
    getOcean(waveHeight, loc.x, loc.z, t);
    for(int i = 0; i < 8; i++){
        maxY = std::max(maxY, rotatePoints[i].y);
    }
    if(maxY < waveHeight){
        return l * w * 2 + l * h * 2 + w * h * 2;
    }

    float minX = 1000000, maxX = -1000000, minZ = 1000000, maxZ = -1000000;
    for(int i = 0; i < 8; i++){
        minX = std::min(minX, rotatePoints[i].x);
        maxX = std::max(maxX, rotatePoints[i].x);
        minZ = std::min(minZ, rotatePoints[i].z);
        maxZ = std::max(maxZ, rotatePoints[i].z);
    }
    float area = 0;
    float dx = 0.1, dy = 0.1;

    for(float i = minX; i < maxX; i += dx){
        for(float j = minZ; j < maxZ; j += dy){
            float waveHeight;
            getOcean(waveHeight, i, j, t);
            std::pair<float , float> y = getY(i, j);

            // 特判 全部在水下 上下表面積
            if(y.first < waveHeight){
                area += dx * dy * 2;
            }
            // 一面在水下
            else if(y.second < waveHeight){
                area += dx * dy;
            }
        }
    }
    return area;
}