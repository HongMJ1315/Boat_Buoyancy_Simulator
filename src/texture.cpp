#include "texture.h"
#include "GLinclude.h"


void GenerateOceanTexture(unsigned char texture[TEXTURE_SIZE][TEXTURE_SIZE][4]){
    for(int i = 0; i < TEXTURE_SIZE; i++){
        for(int j = 0; j < TEXTURE_SIZE; j++){
            texture[i][j][0] = 46;
            texture[i][j][1] = 134;
            texture[i][j][2] = 193;
            texture[i][j][3] = 255.0;
        }
    }
}

void GenerateObjectTexture(unsigned char texture[TEXTURE_SIZE][TEXTURE_SIZE][4]){
    // 44, 62, 80
    for(int i = 0; i < TEXTURE_SIZE; i++){
        for(int j = 0; j < TEXTURE_SIZE; j++){
            texture[i][j][0] = 44;
            texture[i][j][1] = 62;
            texture[i][j][2] = 80;
            texture[i][j][3] = 255.0;
        }
    }
}

void TextureInit(TEXTURE textType, unsigned int *textName, unsigned char texture[TEXTURE_SIZE][TEXTURE_SIZE][4], int width, int height){
    glBindTexture(GL_TEXTURE_2D, textName[textType]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT = repeat texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
}

void SetTexture(TEXTURE textType, unsigned int *textName){
    float diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    float specular[] = { 0.8, 0.8, 0.8, 1.0 };
    float ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    float emission[] = { 0.0, 0.0, 0.0, 1.0 };
    float shininess = 0.0;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textName[textType]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    switch(textType){
        case T_OBJECT:
        shininess = 10;
        specular[0] = 0.3; specular[1] = 0.3; specular[2] = 0.3;
        break;

        case T_OCEAN:
        shininess = 2;
        specular[0] = 0.5; specular[1] = 0.5; specular[2] = 0.5;
        break;

        default:
        break;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}