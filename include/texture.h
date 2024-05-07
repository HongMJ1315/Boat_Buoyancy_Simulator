#ifndef TEXTURE_H
#define TEXTURE_H

#define TEXTURE_SIZE 256
enum TEXTURE{
    T_OCEAN, T_OBJECT, T_NONE
};

void GenerateOceanTexture(unsigned char[TEXTURE_SIZE][TEXTURE_SIZE][4]);

void GenerateObjectTexture(unsigned char[TEXTURE_SIZE][TEXTURE_SIZE][4]);

void TextureInit(TEXTURE, unsigned int *, unsigned char arr[TEXTURE_SIZE][TEXTURE_SIZE][4], int, int);

void SetTexture(TEXTURE textType, unsigned int *textName);

#endif