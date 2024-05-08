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

bool ellipsoid::checkIn(glm::vec3 p){
    // 將點帶入橢球方程式
    // (x - loc.x)^2/l + (y - loc.y)^2 / w + (z - loc.z)^2 / h = 1
    // 
    float result = pow(p.x - loc.x, 2) / l + pow(p.y - loc.y, 2) / w + pow(p.z - loc.z, 2) / h;
    if(result > 1) return 1; // 在球外
    else return 0; // 在球面上(包含球上)
}

glm::vec3 ellipsoid::getABC(float p, float lv, float loc, float *whl, int index){
    // std::cout << p << " " << lv << " " << loc << '\n';
    float a = lv * lv * whl[0] * whl[1] * whl[2] / whl[index];
    float b = 2.0 * (p * lv - lv * loc) * whl[0] * whl[1] * whl[2] / whl[index];
    float c = (p - loc) * (p - loc) * whl[0] * whl[1] * whl[2] / whl[index];
    // std::cout << a << " " << b << " " << c << std::endl;
    return glm::vec3(a, b, c);
};

std::pair<float, float> ellipsoid::getY(float x, float z){
    // 計算法向量
    glm::vec3 v = glm::vec3(0, 1, 0);
    // 旋轉矩陣 旋轉rotate
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(-rotate.x), glm::vec3(1, 0, 0));
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(-rotate.y), glm::vec3(0, 1, 0));
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(-rotate.z), glm::vec3(0, 0, 1));
    // 旋轉v
    v = rotateZ * rotateY * rotateX * glm::vec4(v, 1.0);

    // 計算線起始點
    glm::vec3 p = glm::vec3(x, 0, z);
    // 旋轉起始點
    p = rotateZ * rotateY * rotateX * glm::vec4(p, 1.0);

    // std::cout << p.x << ' ' << p.y << ' ' << p.z << std::endl;
    // std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;
    float whl[] = { l,w, h };
    glm::vec3 X = getABC(p.x, v.x, loc.x, whl, 0);
    glm::vec3 Y = getABC(p.y, v.y, loc.y, whl, 1);
    glm::vec3 Z = getABC(p.z, v.z, loc.z, whl, 2);
    float a = X.x + Y.x + Z.x;
    float b = X.y + Y.y + Z.y;
    float c = X.z + Y.z + Z.z - whl[0] * whl[1] * whl[2];
    // std::cout << "p:" << p.x << " " << p.y << " " << p.z << std::endl;
    // std::cout << "v:" << v.x << " " << v.y << " " << v.z << std::endl;
    // std::cout << "a:" << a << " b:" << b << " c:" << c << std::endl;
    if(b * b - 4 * a * c < 0){
        float t = -b / 2 * a;
        return std::make_pair(p.y + t * v.y, p.y + t * v.y);
    }
    if(b * b - 4 * a * c < 0){
        return std::make_pair(-INF, -INF);
    }
    // 帶入公式解 
    float t1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
    float t2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

    // 帶回t求出y
    float y1 = p.y + t1 * v.y;
    float y2 = p.y + t2 * v.y;

    // std::cout << y1 << " " << y2 << std::endl;
    if(y1 < y2) std::swap(y1, y2);
    return std::make_pair(y1, y2);
    // float y1 = loc.y + w * sqrt(1 - (x - loc.x) * (x - loc.x) / (l * l) - (z - loc.z) * (z - loc.z) / (h * h));
    // float y2 = loc.y - w * sqrt(1 - (x - loc.x) * (x - loc.x) / (l * l) - (z - loc.z) * (z - loc.z) / (h * h));
    // return std::make_pair(y1, y2);
}

void ellipsoid::decomposeEllipsoid(float t){
    decompose.clear();
    float dx = 0.1, dz = 0.1;
    // std::cout << getY(0.1, 3).first << " " << getY(0.1, 3).second << std::endl;

    for(float i = -l + loc.x; i <= l + loc.x; i += dx){
        for(float j = -h + loc.z; j <= h + loc.z; j += dz){
            Cylinder c;
            c.loc = glm::vec2(i, j);
            float waveHeight;
            getOcean(waveHeight, i, j, t);
            // std::cout << getY(i, j).first << " " << getY(i, j).second << std::endl;
            if(getY(i, j).first == -INF || getY(i, j).second == -INF) continue;
            if(std::isnan(getY(i, j).first) || std::isnan(getY(i, j).second)) continue;
            float inWaterY = std::min(waveHeight, getY(i, j).first);
            c.m = (inWaterY - getY(i, j).second) * dx * dz;
            c.h = std::max(inWaterY - getY(i, j).second, 0.0f);
            // std::cout << getY(i, j).first << " " << getY(i, j).second << " " << waveHeight << std::endl;
            if(c.m > 0)
                decompose.push_back(c);
        }
    }
    // std::cout << std::endl;
}

glm::vec3 ellipsoid::getBuoyancyCenter(float t){
    float totalM = 0;
    glm::vec3 result = glm::vec3(0, 0, 0);
    for(auto &c : decompose){
        totalM += c.m;
        // std::cout << c.m << std::endl;
    }
    for(auto &c : decompose){
        result += c.m / totalM * glm::vec3(c.loc.x, getY(c.loc.x, c.loc.y).second + c.h / 2.0, c.loc.y);
    }
    return result;
}

glm::vec3 ellipsoid::getBuoyancyForce(float t){
    float totalM = 0;
    for(auto &c : decompose){
        totalM += c.m;
    }
    return glm::vec3(0, totalM, 0);
}

float ellipsoid::getReflectedArea(float t){
    float area = 0;
    float dx = 0.1, dz = 0.1;
    for(float i = -l + loc.x; i <= l + loc.x; i += dx){
        for(float j = -h + loc.z; j <= h + loc.z; j += dz){
            float waveHeight;
            getOcean(waveHeight, i, j, t);
            std::pair<float, float> y = getY(i, j);

            // 特判 全部在水下 上下表面積
            if(y.first < waveHeight){
                area += dx * dz * 2;
            }
            // 一面在水下
            else if(y.second < waveHeight){
                area += dx * dz;
            }
        }
    }
    return area;
}