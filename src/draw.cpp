#include "draw.h"
#include "ocean.h"
#include "custom_object.h"
#include "ellipsoid.h"
#include <algorithm>
#define BOX_SIZE 21


int keyState[GLFW_KEY_LAST] = { 0 };
int directionKey[4] = { 0 };
bool phyActive = false;
unsigned int textName[10];
unsigned char oceanText[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char emptyText[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char objectText[TEXTURE_SIZE][TEXTURE_SIZE][4];
int viewMode = 0;
std::vector<glm::vec3> buoyancyCenter;


void initTexture(){
    for(int i = 0; i < TEXTURE_SIZE; i++)
        for(int j = 0; j < TEXTURE_SIZE; j++)
            emptyText[i][j][0] = emptyText[i][j][1] =
            emptyText[i][j][2] = emptyText[i][j][3] = 255;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 10);
    glGenTextures(10, textName);
    GenerateOceanTexture(oceanText);
    GenerateObjectTexture(objectText);
    TextureInit(TEXTURE::T_OCEAN, textName, oceanText, TEXTURE_SIZE, TEXTURE_SIZE);
    TextureInit(TEXTURE::T_OBJECT, textName, objectText, TEXTURE_SIZE, TEXTURE_SIZE);
    // TextureInit(TEXTURE::T_WALL, textName, emptyText, TEXTURE_SIZE, TEXTURE_SIZE);
    setGLSLshaders("shader/Phong.vert", "shader/Phong.frag");
    int texLoc = glGetUniformLocation(ReturnProgramID(), "myTex");
    // std::cout << texLoc << std::endl;
    glUniform1i(texLoc, 0);
}
void glInit(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //smooth shading
    glShadeModel(GL_SMOOTH);
}

void setLight(){
    glEnable(SUN_LIGHT);
    GLfloat light_position[] = { 0.0, -1.0, 0.0, 0.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };     // Diffuse light color (RGBA)
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };    // Specular light color (RGBA)
    glLightfv(SUN_LIGHT, GL_POSITION, light_position);
    glLightfv(SUN_LIGHT, GL_AMBIENT, light_ambient);
    glLightfv(SUN_LIGHT, GL_DIFFUSE, light_diffuse);
    glLightfv(SUN_LIGHT, GL_SPECULAR, light_specular);
}

void initObjects(std::vector<object *> &objs, int num){
    for(float i = -MAP_SIZE; i < MAP_SIZE; i += 0.1){
        for(float j = -MAP_SIZE; j < MAP_SIZE; j += 0.1){
            objs.push_back(new cube(glm::vec3(i, 0, j), 0.1, 0.001, 0.1));
            objs.back()->setTexture(TEXTURE::T_OCEAN, textName);
            objs.back()->setName("Ocean");
        }
    }
    objs.push_back(new ellipsoid(glm::vec3(0, 0, 3), 3, 0.6, 0.6));
    objs.back()->setTexture(TEXTURE::T_OBJECT, textName);
    objs.back()->setM(1.0f);
    objs.back()->setName("Ellipsoid");
    // objs.back()->setRotateV(glm::vec3(0, 1, 0));
    objs.push_back(new cube(glm::vec3(4, 0, 0), 3.0, 0.5, 3.0));
    objs.back()->setTexture(TEXTURE::T_OBJECT, textName);
    objs.back()->setM(2.0f);
    objs.back()->setName("Cube");
    // objs.back()->setRotateV(glm::vec3(0, 1, 0));
    objs.push_back(new cube(glm::vec3(0, 0, -3), 1.0, 0.5, 1.0));
    objs.back()->setTexture(TEXTURE::T_OBJECT, textName);
    objs.back()->setM(.4f);
    objs.back()->setName("Cube");

    std::cout << "Done" << std::endl;
}

void move(float dx, float dy, float dz, glm::vec3 &frontPos, glm::vec3 &cameraPos){
    frontPos.x += dx;
    frontPos.y += dy;
    frontPos.z += dz;
    cameraPos.x += dx;
    cameraPos.y += dy;
    cameraPos.z += dz;
}

void update(glm::vec3 &frontPos, glm::vec3 &cameraPos){
    // front and back
    glm::vec3 dl = glm::normalize(glm::vec3(frontPos.x, 0, frontPos.z) - glm::vec3(cameraPos.x, 0, cameraPos.z)) * MOVE_SPEED;
    // right and left
    glm::vec3 dr = glm::normalize(glm::cross(frontPos - cameraPos, glm::vec3(0, 1, 0))) * MOVE_SPEED;
    if(keyState['W']){
        cameraPos += dl * MOVE_SPEED;
        frontPos += dl * MOVE_SPEED;
    }
    if(keyState['S']){
        cameraPos -= dl * MOVE_SPEED;
        frontPos -= dl * MOVE_SPEED;
    }
    // Left
    if(keyState['A']){
        cameraPos -= dr * MOVE_SPEED;
        frontPos -= dr * MOVE_SPEED;
    }
    // Right
    if(keyState['D']){
        cameraPos += dr * MOVE_SPEED;
        frontPos += dr * MOVE_SPEED;
    }


    // Rotate
    if(keyState['I']){
        frontPos = moveCameraUD(cameraPos, frontPos, -ROTATE_SPEED);
    }
    if(keyState['K']){
        frontPos = moveCameraUD(cameraPos, frontPos, ROTATE_SPEED);
    }
    if(keyState['J']){
        frontPos = moveCameraLR(cameraPos, frontPos, -ROTATE_SPEED);
    }
    if(keyState['L']){
        frontPos = moveCameraLR(cameraPos, frontPos, ROTATE_SPEED);
    }

    if(directionKey[0]){
        cameraPos.y += MOVE_SPEED;
        frontPos.y += MOVE_SPEED;
    }
    if(directionKey[1]){
        cameraPos.y -= MOVE_SPEED;
        frontPos.y -= MOVE_SPEED;
    }
}

void drawCoordinateString(glm::vec3 cameraPos, glm::vec3 frontPos, int width, int height, float dt, int fps){
    glUseProgram(0);
    std::string cameraPosSt = "Camera Position: (" + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ", " + std::to_string(cameraPos.z) + ")";
    // std::string frontPosSt = "Front Position: (" + std::to_string(frontPos.x) + ", " + std::to_string(frontPos.y) + ", " + std::to_string(frontPos.z) + ")";
    std::vector<std::string> buoyancyCenterSt;
    for(int i = 0; i < buoyancyCenter.size(); i++){
        buoyancyCenterSt.push_back("Buoyancy Center " + std::to_string(i) + ": (" + std::to_string(buoyancyCenter[i].x) + ", " + std::to_string(buoyancyCenter[i].y) + ", " + std::to_string(buoyancyCenter[i].z) + ")");
    }
    std::string dtFpsSt = "dt: " + std::to_string(dt) + " fps: " + std::to_string(fps);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glViewport(0, 0, width, height);
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    // Set text color
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glRasterPos2f(10, height - 30);
    for(int i = 0; i < cameraPosSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cameraPosSt[i]);
    }
    int nowHeight = height - 60;
    for(auto &i : buoyancyCenterSt){
        glRasterPos2f(10, nowHeight);
        for(int j = 0; j < i.length(); j++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i[j]);
        }
        nowHeight -= 30;
    }
    glRasterPos2f(10, nowHeight);
    for(int i = 0; i < dtFpsSt.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, dtFpsSt[i]);
    }
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawSingleView(std::vector<object *> &objs, int width, int height, glm::vec3 &cameraPos, glm::vec3 &frontPos){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, frontPos.x, frontPos.y, frontPos.z, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setLight();
    for(auto &obj : objs){
        obj->draw();
    }
}

void drawMultiView(std::vector<object *> &objs, int width, int height, glm::vec3 &cameraPos, glm::vec3 &frontPos){
    int viewportWidth = width / 2;
    int viewportHeight = height / 2;

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    // Top Left View 
    glLoadIdentity();
    glViewport(0, height / 2, viewportWidth, viewportHeight);
    gluPerspective(60.0f, (float) width / (float) height, 0.1f, 10000.0f);
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, frontPos.x, frontPos.y, frontPos.z, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        obj->draw();
    }

    // Top Right View
    glLoadIdentity();
    glViewport(width / 2, height / 2, viewportWidth, viewportHeight);
    glOrtho(-width / 50, width / 50, -height / 50, height / 50, -1000, 1000);
    gluLookAt(0 - 1, 0, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        if(obj->getName() == "BWall") continue;
        obj->draw();
    }

    // Bottom Left View
    glLoadIdentity();
    glViewport(0, 0, viewportWidth, viewportHeight);
    glOrtho(-width / 50, width / 50, -height / 50, height / 50, -1000, 1000);
    gluLookAt(0, 0, 0 - 1, 0, 0, 0, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        if(obj->getName() == "LWall") continue;
        obj->draw();
    }

    // Bottom Right View
    glLoadIdentity();
    glViewport(width / 2, 0, viewportWidth, viewportHeight);
    glOrtho(-width / 50, width / 50, -height / 50, height / 50, -1000, 1000);
    gluLookAt(0 - 0.001f, 0 + 1, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f);
    setLight();
    for(auto &obj : objs){
        obj->draw();
    }
}



void display(GLFWwindow *window, int width, int height, float dt, int fps, glm::vec3 &cameraPos, glm::vec3 &frontPos, std::vector<object *> &objs){
    // glUseProgram(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawCoordinateString(cameraPos, frontPos, width, height, dt, fps);

    glUseProgram(ReturnProgramID());
    if(viewMode == 0)
        drawSingleView(objs, width, height, cameraPos, frontPos);
    else if(viewMode == 1)
        drawMultiView(objs, width, height, cameraPos, frontPos);
    glfwSwapBuffers(window);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << std::hex << "OpenGL error: " << error << std::endl;
    }
}


void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        keyState[key] = true;
        if(key == GLFW_KEY_P){
            phyActive = !phyActive;
        }
        if(key == GLFW_KEY_V){
            viewMode = (viewMode + 1) % 2;
        }
        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = true;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = true;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = true;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = true;
            break;

        }
    }
    else if(action == GLFW_RELEASE){
        keyState[key] = false;
        switch(key){
            case GLFW_KEY_UP:
            directionKey[0] = false;
            break;
            case GLFW_KEY_DOWN:
            directionKey[1] = false;
            break;
            case GLFW_KEY_LEFT:
            directionKey[2] = false;
            break;
            case GLFW_KEY_RIGHT:
            directionKey[3] = false;
            break;
        }
    }
}

void updatePhysics(float dt, float t, std::vector<object *> &objs){
    // /*
    for(auto &obj : objs){
        if(obj->getName() == "Ocean"){
            getOcean(obj, t);
        }
    }

    // get ellipsoid object
    std::vector<ellipsoid *> ellipsoidObjs;
    std::vector<cube *> cubeObjs;
    for(auto &i : objs){
        if(dynamic_cast<ellipsoid *>(i) != nullptr){
            ellipsoidObjs.push_back(dynamic_cast<ellipsoid *>(i));
        }
        if(dynamic_cast<cube *>(i) != nullptr && i->getName() != "Ocean"){
            cubeObjs.push_back(dynamic_cast<cube *>(i));
        }
    }

    if(buoyancyCenter.size() != ellipsoidObjs.size() + cubeObjs.size()){
        buoyancyCenter.resize(ellipsoidObjs.size() + cubeObjs.size());
    }

    // 更新ellipsoid的buoyancyCenter和buoyancyForce
    for(int i = 0; i < ellipsoidObjs.size(); i++){
        buoyancyCenter[i] = ellipsoidObjs[i]->getBuoyancyCenter(t);
        glm::vec3 buoyancyForce = ellipsoidObjs[i]->getBuoyancyForce(t);


        // 重力施加在loc上 浮力施加在buoyancyCenter上 計算角加速度
        // 重力
        glm::vec3 m = glm::vec3(0, -1, 0) * ellipsoidObjs[i]->getM();
        // 浮力
        glm::vec3 b = buoyancyForce;

        // 計算角加速度
        glm::vec3 torque = glm::cross(buoyancyCenter[i] - ellipsoidObjs[i]->getLoc(), b) ;
        ellipsoidObjs[i]->setRotateA(torque / ellipsoidObjs[i]->getM());

        // 計算重力和浮力垂直方向的加速度
        glm::vec3 a = m + b;
        ellipsoidObjs[i]->setA(a / ellipsoidObjs[i]->getM());
    }

    // 更新cube的buoyancyCenter和buoyancyForce
    for(int i = 0; i < cubeObjs.size(); i++){
        buoyancyCenter[i + ellipsoidObjs.size()] = cubeObjs[i]->getBuoyancyCenter(t);
        glm::vec3 buoyancyForce = cubeObjs[i]->getBuoyancyForce(t);

        // 重力施加在loc上 浮力施加在buoyancyCenter上 計算角加速度
        // 重力
        glm::vec3 m = glm::vec3(0, -1, 0) * cubeObjs[i]->getM();
        // 浮力
        glm::vec3 b = buoyancyForce;

        // 計算角加速度
        glm::vec3 torque = glm::cross(buoyancyCenter[i + ellipsoidObjs.size()] - cubeObjs[i]->getLoc(), b);
        cubeObjs[i]->setRotateA(torque / cubeObjs[i]->getM());

        // 計算重力和浮力垂直方向的加速度
        glm::vec3 a = m + b;
        cubeObjs[i]->setA(a / cubeObjs[i]->getM());

    }

    // 計算水對物體的阻尼力

    // */
    for(auto &obj : objs){
        if(obj->getName() != "Ocean")
            obj->update(dt);
    }

}
