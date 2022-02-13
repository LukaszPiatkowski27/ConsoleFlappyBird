#pragma once

#include<fstream>
#include<iostream>
#include"ConsoleClass.h"

class VectorRGB{
    public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    VectorRGB():R{ 0 },
                G{ 0 },
                B{ 0 }{}
    VectorRGB(unsigned char _r, unsigned char _g, unsigned char _b):R{ _r },
                                                                    G{ _g },
                                                                    B{ _b }{}
    ~VectorRGB(){}
};

void ReadBMP(char* filename, vector<VectorRGB> &vRGB)
{
    FILE* f = fopen(filename, "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); //54-bajtowy nag³ówek
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char temp;
    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            //(B, G, R) to (R, G, B)
            temp = data[j];
            data[j] = data[j+2];
            data[j+2] = temp;
            VectorRGB * tempRGB = new VectorRGB(data[j],data[j+1],data[j+2]);
            vRGB.push_back(*tempRGB);
            delete tempRGB;
        }
    }

    fclose(f);
    delete data;
}
