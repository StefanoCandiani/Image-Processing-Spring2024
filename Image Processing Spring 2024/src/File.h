//
// Created by Stefano Candiani on 3/13/24.
//

#ifndef PROJECT2_FILE_H
#define PROJECT2_FILE_H
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Header {
    Header();
    Header(char idLength, char colorMapType, char imgType, short colorMapOrigin, short colorMapLength,
           char colorMapDepth, short xOrigin, short yOrigin, short imgWidth, short imgHeight, char pixDepth,
           char imgDescriptor);
    void displayHeader();
    char idLength;
    char colorMapType;
    char imgType;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short imgWidth;
    short imgHeight;
    char pixDepth;
    char imgDescriptor;
};

class Photoshop {
    private:
        //
    public:
        Photoshop();
        Header storeHeader(fstream& fileInfo);
        void storeData(fstream &fileInfo, Header &information, vector<vector<unsigned char>> &data);
        void writeFile(fstream &copyFile, Header &information, vector<vector<unsigned char>> &data);
        vector<unsigned char> multiply(vector<unsigned char> pixel1, vector<unsigned char> pixel2);
        vector<unsigned char> scale(vector<unsigned char> pixel1, vector<int> scaler);
        vector<unsigned char> subtract(vector<unsigned char> pixel1, vector<unsigned char> pixel2);
        vector<unsigned char> add(vector<unsigned char> pixel1, vector<unsigned char> pixel2);
        vector<unsigned char> screen(vector<unsigned char> pixel1, vector<unsigned char> pixel2);
        vector<unsigned char> overlay(vector<unsigned char> pixel1, vector<unsigned char> pixel2);
        float nC(int channel);
        unsigned char dNC(float value);
        void fileChecker(string path1, string path2);


};


#endif //PROJECT2_FILE_H
