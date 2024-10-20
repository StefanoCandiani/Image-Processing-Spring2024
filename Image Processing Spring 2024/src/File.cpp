//
// Created by Stefano Candiani on 3/13/24.
//
#include "File.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

Header::Header() {

}

Header::Header(char idLength, char colorMapType, char imgType, short colorMapOrigin, short colorMapLength,
               char colorMapDepth, short xOrigin, short yOrigin, short imgWidth, short imgHeight, char pixDepth,
               char imgDescriptor) {
    this -> idLength = idLength;
    this -> colorMapType = colorMapType;
    this -> imgType = imgType;
    this -> colorMapOrigin = colorMapOrigin;
    this -> colorMapLength = colorMapLength;
    this -> colorMapDepth = colorMapDepth;
    this -> xOrigin = xOrigin;
    this -> yOrigin = yOrigin;
    this -> imgWidth = imgWidth;
    this -> imgHeight = imgHeight;
    this -> pixDepth = pixDepth;
    this -> imgDescriptor = imgDescriptor;
}

void Header::displayHeader() {
    cout << (int)idLength << endl;
    cout << (int)colorMapType << endl;
    cout << (int)imgType << endl;
    cout << colorMapOrigin << endl;
    cout << (int)colorMapLength << endl;
    cout << (int)colorMapDepth << endl;
    cout << xOrigin << endl;
    cout << yOrigin << endl;
    cout << (int)imgWidth << endl;
    cout << (int)imgHeight << endl;
    cout << (int)pixDepth << endl;
    cout << (int)imgDescriptor << endl;
}

Photoshop::Photoshop() {

}

Header Photoshop::storeHeader(fstream& fileInfo) {
    char idLength;
    fileInfo.read((char*)(&idLength), 1);
    char colorMapType;
    fileInfo.read((char*)(&colorMapType), 1);
    char dataTypeCode;
    fileInfo.read((char*)(&dataTypeCode), 1);
    short colorMapOrigin;
    fileInfo.read((char*)(&colorMapOrigin), 2);
    short colorMapLength;
    fileInfo.read((char*)(&colorMapLength), 2);
    char colorMapDepth;
    fileInfo.read((char*)(&colorMapDepth), 1);
    short xOrigin;
    fileInfo.read((char*)(&xOrigin), 2);
    short yOrigin;
    fileInfo.read((char*)(&yOrigin), 2);
    short width;
    fileInfo.read((char*)(&width), 2);
    short height;
    fileInfo.read((char*)(&height), 2);
    char bitsPerPixel;
    fileInfo.read((char*)(&bitsPerPixel), 1);
    char imageDescriptor;
    fileInfo.read((char*)(&imageDescriptor), 1);
    Header fileHeader(idLength, colorMapType, dataTypeCode, colorMapOrigin, colorMapLength, colorMapDepth,
                      xOrigin, yOrigin, width, height, bitsPerPixel, imageDescriptor);

    return fileHeader;
}

void Photoshop::storeData(fstream &fileInfo, Header &information, vector<vector<unsigned char>> &data) {
    vector<unsigned char> bgr = {0,0,0};
    for(int i = 0; i < ((information.imgWidth)*(information.imgHeight)); i++){
        fileInfo.read((char*)(&bgr.at(0)), sizeof(bgr.at(0)));
        fileInfo.read((char*)(&bgr.at(1)), sizeof(bgr.at(1)));
        fileInfo.read((char*)(&bgr.at(2)), sizeof(bgr.at(2)));
        data.push_back(bgr);
    }
}

void Photoshop::writeFile(fstream &copyFile, Header &information, vector<vector<unsigned char>> &data) {
    copyFile.write((char*)(&information.idLength), 1);
    copyFile.write((char*)(&information.colorMapType), 1);
    copyFile.write((char*)(&information.imgType), 1);
    copyFile.write((char*)(&information.colorMapOrigin), 2);
    copyFile.write((char*)(&information.colorMapLength), 2);
    copyFile.write((char*)(&information.colorMapDepth), 1);
    copyFile.write((char*)(&information.xOrigin), 2);
    copyFile.write((char*)(&information.yOrigin), 2);
    copyFile.write((char*)(&information.imgWidth), 2);
    copyFile.write((char*)(&information.imgHeight), 2);
    copyFile.write((char*)(&information.pixDepth), 1);
    copyFile.write((char*)(&information.imgDescriptor), 1);
    for(int i = 0; i < data.size(); i++){
        copyFile.write((char*) (&data.at(i).at(0)), sizeof(data.at(i).at(0)));
        copyFile.write((char*) (&data.at(i).at(1)), sizeof(data.at(i).at(1)));
        copyFile.write((char*) (&data.at(i).at(2)), sizeof(data.at(i).at(2)));
    }
}

/// Multiply individual pixels
vector<unsigned char> Photoshop::multiply(vector<unsigned char> pixel1, vector<unsigned char> pixel2) {
    float b1, b2, g1, g2, r1, r2;
    vector<unsigned char> retPixel = {0, 0, 0};
    b1 = pixel1.at(0);  b2 = pixel2.at(0);
    g1 = pixel1.at(1);  g2 = pixel2.at(1);
    r1 = pixel1.at(2);  r2 = pixel2.at(2);
    retPixel.at(0) = (unsigned char)((((float)((float)b1/255)) * ((float)((float)b2/255)) * 255) + 0.5f);
    retPixel.at(1) = (unsigned char)((((float)((float)g1/255)) * ((float)((float)g2/255)) * 255) + 0.5f);
    retPixel.at(2) = (unsigned char)((((float)((float)r1/255)) * ((float)((float)r2/255)) * 255) + 0.5f);
    return retPixel;
}

/// Scale individual pixels
vector<unsigned char> Photoshop::scale(vector<unsigned char> pixel1, vector<int> scaler) {
    float b1, sb2, bfin, g1, sg2, gfin, r1, sr2, rfin;
    vector<unsigned char> retPixel = {0, 0, 0};
    b1 = pixel1.at(0);  sb2 = scaler.at(0);
    g1 = pixel1.at(1);  sg2 = scaler.at(1);
    r1 = pixel1.at(2);  sr2 = scaler.at(2);
    bfin = b1 * sb2; gfin = g1 * sg2; rfin = r1 * sr2;
    if(bfin < 0) { bfin = 0; } else if (gfin < 0) { gfin = 0; } else if (rfin < 0) { rfin = 0; }
    if(bfin > 255) { bfin = 255; } else if (gfin > 255) { gfin = 255; } else if (rfin > 255) { rfin = 255; }
    retPixel.at(0) = (unsigned char)bfin;
    retPixel.at(1) = (unsigned char)gfin;
    retPixel.at(2) = (unsigned char)rfin;

    return retPixel;
}

/// Subtract individual pixels
vector<unsigned char> Photoshop::subtract(vector<unsigned char> pixel1, vector<unsigned char> pixel2) {
    int b1, b2, g1, g2, r1, r2, bfin, gfin, rfin;
    vector<unsigned char> retPixel = {0, 0, 0};
    b1 = (int)pixel1.at(0);  b2 = (int)pixel2.at(0);
    g1 = (int)pixel1.at(1);  g2 = (int)pixel2.at(1);
    r1 = (int)pixel1.at(2);  r2 = (int)pixel2.at(2);
    bfin = b1 - b2; gfin = g1 - g2; rfin = r1 - r2;
    if(bfin < 0) { bfin = 0; } if (gfin < 0) { gfin = 0; } if (rfin < 0) { rfin = 0; }
    if(bfin > 255) { bfin = 255; } if (gfin > 255) { gfin = 255; } if (rfin > 255) { rfin = 255; }
    retPixel.at(0) = (unsigned char)bfin;
    retPixel.at(1) = (unsigned char)gfin;
    retPixel.at(2) = (unsigned char)rfin;
    return retPixel;

}

/// Add individual pixels
vector<unsigned char> Photoshop::add(vector<unsigned char> pixel1, vector<unsigned char> pixel2) {
    int b1, b2, g1, g2, r1, r2, bfin, gfin, rfin;
    vector<unsigned char> retPixel = {0, 0, 0};
    b1 = (int)pixel1.at(0);  b2 = (int)pixel2.at(0);
    g1 = (int)pixel1.at(1);  g2 = (int)pixel2.at(1);
    r1 = (int)pixel1.at(2);  r2 = (int)pixel2.at(2);
    bfin = b1 + b2; gfin = g1 + g2; rfin = r1 + r2;
    if(bfin < 0) { bfin = 0; } if (gfin < 0) { gfin = 0; } if (rfin < 0) { rfin = 0; }
    if(bfin > 255) { bfin = 255; } if (gfin > 255) { gfin = 255; } if (rfin > 255) { rfin = 255; }
    retPixel.at(0) = (unsigned char)bfin;
    retPixel.at(1) = (unsigned char)gfin;
    retPixel.at(2) = (unsigned char)rfin;
    return retPixel;

}

/// Screen individual pixel
vector<unsigned char> Photoshop::screen(vector<unsigned char> pixel1, vector<unsigned char> pixel2) {
    float b1, b2, g1, g2, r1, r2;
    vector<unsigned char> retPixel = {0, 0, 0};
    b1 = pixel1.at(0);  b2 = pixel2.at(0);
    g1 = pixel1.at(1);  g2 = pixel2.at(1);
    r1 = pixel1.at(2);  r2 = pixel2.at(2);
    retPixel.at(0) = (unsigned char)(((1 - ((1 - (float)((float)b1/255))*(1 - (float)((float)b2/255))))*255)+0.5f);
    retPixel.at(1) = (unsigned char)(((1 - ((1 - (float)((float)g1/255))*(1 - (float)((float)g2/255))))*255)+0.5f);
    retPixel.at(2) = (unsigned char)(((1 - ((1 - (float)((float)r1/255))*(1 - (float)((float)r2/255))))*255)+0.5f);
    return retPixel;
}

/// Overlay individual pixels
vector<unsigned char> Photoshop::overlay(vector<unsigned char> pixel1, vector<unsigned char> pixel2) {
    float b1, b2, g1, g2, r1, r2, bfin, gfin, rfin;
    vector<unsigned char> retPixel = {0, 0, 0};
    b1 = nC((int)pixel1.at(0));  b2 = nC((int)pixel2.at(0));
    g1 = nC((int)pixel1.at(1));  g2 = nC((int)pixel2.at(1));
    r1 = nC((int)pixel1.at(2));  r2 = nC((int)pixel2.at(2));
    if(b2 <= 0.5f) {
        bfin = dNC(2 * b1 * b2);
        retPixel.at(0) = bfin;
    } else {
        bfin = dNC(1 - (2 * (1 - b1) * (1 - b2)));
        retPixel.at(0) = bfin;
    }
    if(g2 <= 0.5f) {
        gfin = dNC(2 * g1 * g2);
        retPixel.at(1) = gfin;
    } else {
        gfin = dNC(1 - (2 * (1 - g1) * (1 - g2)));
        retPixel.at(1) = gfin;
    }
    if(r2 <= 0.5f) {
        rfin = dNC(2 * r1 * r2);
        retPixel.at(2) = rfin;
    } else {
        rfin = dNC(1 - (2 * (1 - r1) * (1 - r2)));
        retPixel.at(2) = rfin;
    }
    return retPixel;
}

/// Normalize Channel
float Photoshop::nC(int channel) {
    return (float)channel/255.0f;
}

/// Denormalize Channel
unsigned char Photoshop::dNC(float value) {
    return (unsigned char)((value*255.0f)+0.5f);
}

/// File checker
void Photoshop::fileChecker(string path1, string path2) {
    fstream readFile1(path1, ios_base::binary | ios_base::in);
    fstream readFile2(path2, ios_base::binary | ios_base::in);

    Header header1 = this -> storeHeader(readFile1);
    Header header2 = this -> storeHeader(readFile2);

    vector<vector<unsigned char>> example;
    vector<vector<unsigned char>> output;

    this -> storeData(readFile1, header1, example);
    this -> storeData(readFile2, header2, output);

    cout << "Size: " << example.size() << endl;

    for (int i = 0; i < example.size(); i++) {
        if ((example.at(i) == output.at(i)) && i < 50) {
            cout << "Not equal at pixel: " << i << endl;
            cout << "b: " << (float) example.at(i).at(0) << ", g: " << (float) example.at(i).at(1) << ", r: "
                 << (float) example.at(i).at(2) << endl;
            cout << "b: " << (float) output.at(i).at(0) << ", g: " << (float) output.at(i).at(1) << ", r: "
                 << (float) output.at(i).at(2) << endl;
        }
    }

    readFile1.close();  readFile2.close();
}