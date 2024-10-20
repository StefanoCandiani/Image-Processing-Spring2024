#include <iostream>
#include <fstream>
#include <vector>
#include "File.h"
using namespace std;

bool checkIfTGA(string fileName) {
    if(fileName.size() < 4) {
        return false;
    } else {
        return (fileName.find(".tga") == fileName.size() - 4);
    }
}

bool fileExist(string fileName) {
    fstream testFile(fileName, ios_base::binary | ios_base::in);
    return testFile.is_open();
}

bool validMethodCheck(string method) {
    if(method == "multiply" || method == "subtract" || method == "overlay" || method == "screen" || method == "combine"
    || method == "flip" || method == "onlyred" || method == "onlygreen" || method == "onlyblue" || method == "addred"
    || method == "addgreen" || method == "addblue" || method == "scalered" || method == "scalegreen" || method == "scaleblue") {
        return true;
    }
    return false;
}

int numAddArg(string method) {
    if(method == "multiply" || method == "subtract" || method == "overlay" || method == "screen" || method == "addred"
    || method == "addgreen" || method == "addblue" || method == "scalered" || method == "scalegreen" || method == "scaleblue"){
        return 1;
    } else if(method == "combine") {
        return 2;
    }
    return 0;
}

void imageActionOneFile(vector<vector<unsigned char>> &input1Pixels, vector<vector<unsigned char>> &finalPixels,
                        string method, Photoshop app) {
    if(method == "flip") {
        int counter = input1Pixels.size() - 1;
        for(int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(counter) = input1Pixels.at(i);
        counter--;
        }
    } else if(method == "onlyred") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = {input1Pixels.at(i).at(2), input1Pixels.at(i).at(2), input1Pixels.at(i).at(2)};
        }
    } else if(method == "onlygreen") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = {input1Pixels.at(i).at(1), input1Pixels.at(i).at(1), input1Pixels.at(i).at(1)};
        }
    } else if(method == "onlyblue") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = {input1Pixels.at(i).at(0), input1Pixels.at(i).at(0), input1Pixels.at(i).at(0)};
        }
    }
}

void imageActionTwoFiles(vector<vector<unsigned char>> &input1Pixels, vector<vector<unsigned char>> &input2Pixels,
                         vector<vector<unsigned char>> &finalPixels, string method, Photoshop app) {
    if(method == "multiply") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.multiply(input1Pixels.at(i), input2Pixels.at(i)));
        }
    } else if(method == "subtract") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.subtract(input1Pixels.at(i), input2Pixels.at(i)));
        }
    } else if(method == "overlay") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.overlay(input1Pixels.at(i), input2Pixels.at(i)));
        }
    } else if(method == "screen") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.screen(input1Pixels.at(i), input2Pixels.at(i)));
        }
    }
}

void imageActionThreeFiles(vector<vector<unsigned char>> &input1Pixels, vector<vector<unsigned char>> &input2Pixels,
                           vector<vector<unsigned char>> &input3Pixels, vector<vector<unsigned char>> &finalPixels,
                           string method, Photoshop app) {
    if(method == "combine") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = {input1Pixels.at(i).at(0), input2Pixels.at(i).at(1), input3Pixels.at(i).at(2)};
        }
    }
}

void imageActionNum(vector<vector<unsigned char>> &input1Pixels, vector<vector<unsigned char>> &finalPixels,
                    string method, Photoshop app, unsigned char value) {
    if(method == "addred") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.add(input1Pixels.at(i), {0,0,value}));
        }
    } else if(method == "addgreen") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.add(input1Pixels.at(i), {0,value,0}));
        }
    } else if(method == "addblue") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.add(input1Pixels.at(i), {value,0,0}));
        }
    } else if(method == "scalered") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.scale(input1Pixels.at(i), {1,1,value}));
        }
    } else if(method == "scalegreen") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.scale(input1Pixels.at(i), {1,value,1}));
        }
    } else if(method == "scaleblue") {
        for (int i = 0; i < input1Pixels.size(); i++) {
            finalPixels.at(i) = (app.scale(input1Pixels.at(i), {value,1,1}));
        }
    }
}


int handleCalls (int start, int argc, char* argv[]) {

    Photoshop app;
    Header inputFile1;
    Header inputFile2;
    Header inputFile3;
    vector<vector<unsigned char>> tempFinalPixels;
    vector<vector<unsigned char>> finalPixels;

    for(int i = start; i < argc; i++) {

        if(!validMethodCheck((string)argv[i])) {
            cout << "Invalid method name." << endl;
            return 1;
        }

        if(numAddArg((string)(argv[i])) == 1 && argc < i + 2
           || numAddArg((string)argv[i]) == 2 && argc < i + 3) {
            cout << "Missing argument." << endl;
            return 1;
        }

        if((string)argv[i] == "multiply" || (string)argv[i] == "subtract" || (string)argv[i] == "screen"
           || (string)argv[i] == "overlay" ) {

            // Error checks
            if(!checkIfTGA((string)argv[i + 1])){
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            } else if (!fileExist((string) argv[i + 1])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }

            fstream readFile1((string)argv[i - 1], ios_base::binary | ios_base::in);
            fstream readFile2((string)argv[i + 1], ios_base::binary | ios_base::in);

            vector<vector<unsigned char>> input1Pixels;
            vector<vector<unsigned char>> input2Pixels;

            inputFile1 = app.storeHeader(readFile1);
            inputFile2 = app.storeHeader(readFile2);

            app.storeData(readFile1, inputFile1, input1Pixels);
            app.storeData(readFile2, inputFile2, input2Pixels);

            if(i < 4) {
                tempFinalPixels.resize(input1Pixels.size());
                finalPixels.resize(input2Pixels.size());}

            if(i == 3) {
                imageActionTwoFiles(input1Pixels, input2Pixels, tempFinalPixels, (string)argv[i], app);
            } else if (i > 3) {
                imageActionTwoFiles(tempFinalPixels, input2Pixels, finalPixels, (string)argv[i], app);
            }

            readFile1.close();   readFile2.close();

            i += 1;

        } else if ((string)argv[i] == "combine") {

            // Error Checks
            if(!checkIfTGA((string)argv[i + 1]) || !checkIfTGA((string)argv[i + 2])){
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            } else if (!fileExist((string) argv[i + 1]) || !fileExist((string) argv[i + 2])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }

            fstream readFile1((string)argv[i - 1], ios_base::binary | ios_base::in);
            fstream readFile2((string)argv[i + 1], ios_base::binary | ios_base::in);
            fstream readFile3((string)argv[i + 2], ios_base::binary | ios_base::in);

            vector<vector<unsigned char>> input1Pixels;
            vector<vector<unsigned char>> input2Pixels;
            vector<vector<unsigned char>> input3Pixels;

            inputFile1 = app.storeHeader(readFile1);
            inputFile2 = app.storeHeader(readFile2);
            inputFile3 = app.storeHeader(readFile3);

            app.storeData(readFile1, inputFile1, input1Pixels);
            app.storeData(readFile2, inputFile2, input2Pixels);
            app.storeData(readFile3, inputFile3, input3Pixels);

            if(i < 4) {
                tempFinalPixels.resize(input1Pixels.size());
                finalPixels.resize(input1Pixels.size());
            }

            if(i == 3) {
                imageActionThreeFiles(input1Pixels, input2Pixels,input3Pixels, tempFinalPixels, (string)argv[i], app);
            } else if (i > 3) {
                imageActionThreeFiles(tempFinalPixels, input2Pixels, input3Pixels, finalPixels, (string)argv[i], app);
            }

            readFile1.close();   readFile2.close();   readFile3.close();

            i += 2;
        } else if ((string)argv[i] == "addred" || (string)argv[i] == "addgreen" || (string)argv[i] == "addblue"
                   || (string)argv[i] == "scalered" || (string)argv[i] == "scalegreen" || (string)argv[i] == "scaleblue") {
            if(!isdigit(*argv[i + 1])) {
                cout << "Invalid argument, expected number." << endl;
                return 1;
            }

            fstream readFile1((string)argv[i - 1], ios_base::binary | ios_base::in);

            vector<vector<unsigned char>> input1Pixels;

            inputFile1 = app.storeHeader(readFile1);

            app.storeData(readFile1, inputFile1, input1Pixels);

            if(i < 4) {
                tempFinalPixels.resize(input1Pixels.size());
                finalPixels.resize(input1Pixels.size());
            }

            if(i == 3) {
                imageActionNum(input1Pixels,tempFinalPixels, (string)argv[i], app, *argv[i + 1]);
            } else if (i > 3) {
                imageActionNum(tempFinalPixels, finalPixels, (string)argv[i], app, *argv[i + 1]);
            }

            readFile1.close();

            i += 1;
        } else if((string)argv[i] == "flip" || (string)argv[i] == "onlyred" || (string)argv[i] == "onlygreen"
               || (string)argv[i] == "onlyblue") {

            fstream readFile1((string)argv[i - 1], ios_base::binary | ios_base::in);

            vector<vector<unsigned char>> input1Pixels;

            inputFile1 = app.storeHeader(readFile1);

            app.storeData(readFile1, inputFile1, input1Pixels);

            if(i < 4) {
                tempFinalPixels.resize(input1Pixels.size());
                finalPixels.resize(input1Pixels.size());
            }

            if(i == 3) {
                imageActionOneFile(input1Pixels,tempFinalPixels, (string)argv[i], app);
            } else if (i > 3) {
                imageActionOneFile(tempFinalPixels, finalPixels, (string)argv[i], app);
            }

            readFile1.close();
        }
    }

    if(argc > 3 && argc <= 5) {
        fstream copyFile((string) argv[1], ios_base::binary | ios_base::out);

        app.writeFile(copyFile, inputFile1, tempFinalPixels);

        copyFile.close();
    } else if(argc > 5) {
        fstream copyFile((string) argv[1], ios_base::binary | ios_base::out);
        app.writeFile(copyFile, inputFile1, finalPixels);

        copyFile.close();
    }

    return 0;
}

int main(int argc, const char* argv[]) {

/// Part 2

string helpMessage = "Project 2: Image Processing, Spring 2024\n\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]";
// Simple call
    for(int i = 0; i < 3; i++) { /*cout << i << " " << (string)argv[i] << endl; }*/
        // No command provided message:
        if(i == 0 && argc == 1) {
            cout << helpMessage << endl;
        }
        // Help requested or checking if file is tga
        if(i == 1 && argc > 1) {
            if((string)argv[1] == "--help") {
                cout << helpMessage << endl;
                return 0;
            } else if (!checkIfTGA((string)argv[1])) {
                cout << "Invalid file name." << endl;
                return 0;
            }
        }

        // Checking if input file is not tga or if file exists
        if (i == 2 && argc > 1) {
            if (!checkIfTGA((string) argv[2])) {
                cout << "Invalid file name." << endl;
                return 0;
            } else if(!fileExist((string)argv[2])) {
                cout << "File does not exist." << endl;
                return 0;
            } else if(argc == 3) {
                cout << "Invalid method name." << endl;
                return 0;
            }
        }
    }

// Method and complex method handling

    handleCalls(3, argc, (char**)argv);

//        fstream trackImg(argv[1], ios_base::binary | ios_base::out);

/// Part 1
//    Photoshop practice;
//
///// Multiply test (Part1)
//
//    fstream readFileP1_1("./input/layer1.tga", ios_base::binary | ios_base::in);
//    fstream readFileP1_2("./input/pattern1.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP1("./output/part1.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP1 = practice.storeHeader(readFileP1_1);
//    Header pattern1HeaderP1 = practice.storeHeader(readFileP1_2);
//
//    vector<vector<unsigned char>> layer1PixelsP1;
//    vector<vector<unsigned char>> pattern1PixelsP1;
//    vector<vector<unsigned char>> finalPixelsP1;
//
//    practice.storeData(readFileP1_1, layer1HeaderP1, layer1PixelsP1);
//    practice.storeData(readFileP1_2, pattern1HeaderP1, pattern1PixelsP1);
//
//    for(int i = 0; i < layer1PixelsP1.size(); i++) {
//        finalPixelsP1.push_back(practice.multiply(layer1PixelsP1.at(i), pattern1PixelsP1.at(i)));
//    }
//
//    practice.writeFile(copyFileP1,layer1HeaderP1,finalPixelsP1);
//
//    readFileP1_1.close();   readFileP1_2.close();   copyFileP1.close();
//
///// Subtract test (Part2)
//
//    fstream readFileP2_1("./input/car.tga", ios_base::binary | ios_base::in);
//    fstream readFileP2_2("./input/layer2.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP2("./output/part2.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP2 = practice.storeHeader(readFileP2_1);
//    Header pattern1HeaderP2 = practice.storeHeader(readFileP2_2);
//
//    vector<vector<unsigned char>> layer1PixelsP2;
//    vector<vector<unsigned char>> pattern1PixelsP2;
//    vector<vector<unsigned char>> finalPixelsP2;
//
//    practice.storeData(readFileP2_1, layer1HeaderP2, layer1PixelsP2);
//    practice.storeData(readFileP2_2, pattern1HeaderP2, pattern1PixelsP2);
//
//    for(int i = 0; i < layer1PixelsP2.size(); i++) {
//        finalPixelsP2.push_back(practice.subtract(layer1PixelsP2.at(i), pattern1PixelsP2.at(i)));
//    }
//
//    practice.writeFile(copyFileP2,layer1HeaderP2,finalPixelsP2);
//
//    readFileP2_1.close();   readFileP2_2.close();   copyFileP2.close();
//
///// Multiply and Screen test (Part3)
//
//    fstream readFileP3_1("./input/layer1.tga", ios_base::binary | ios_base::in);
//    fstream readFileP3_2("./input/pattern2.tga", ios_base::binary | ios_base::in);
//    fstream readFileP3_3("./input/text.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP3("./output/part3.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP3 = practice.storeHeader(readFileP3_1);
//    Header pattern1HeaderP3 = practice.storeHeader(readFileP3_2);
//    Header pattern2HeaderP3 = practice.storeHeader(readFileP3_3);
//
//    vector<vector<unsigned char>> layer1PixelsP3;
//    vector<vector<unsigned char>> pattern1PixelsP3;
//    vector<vector<unsigned char>> pattern2PixelsP3;
//    vector<vector<unsigned char>> finalPixelsP3;
//
//    practice.storeData(readFileP3_1, layer1HeaderP3, layer1PixelsP3);
//    practice.storeData(readFileP3_2, pattern1HeaderP3, pattern1PixelsP3);
//    practice.storeData(readFileP3_3, pattern2HeaderP3, pattern2PixelsP3);
//
//
//    for(int i = 0; i < layer1PixelsP3.size(); i++){
//        finalPixelsP3.push_back(practice.multiply(layer1PixelsP3.at(i), pattern1PixelsP3.at(i)));
//        finalPixelsP3.at(i) = practice.screen(finalPixelsP3.at(i), pattern2PixelsP3.at(i));
//    }
//
//    practice.writeFile(copyFileP3,layer1HeaderP3,finalPixelsP3);
//
//    readFileP3_1.close();   readFileP3_2.close();   readFileP3_3.close();   copyFileP3.close();
//
///// Multiply and subtract test (Part4)
//
//    fstream readFileP4_1("./input/layer2.tga", ios_base::binary | ios_base::in);
//    fstream readFileP4_2("./input/circles.tga", ios_base::binary | ios_base::in);
//    fstream readFileP4_3("./input/pattern2.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP4("./output/part4.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP4 = practice.storeHeader(readFileP4_1);
//    Header pattern1HeaderP4 = practice.storeHeader(readFileP4_2);
//    Header pattern2HeaderP4 = practice.storeHeader(readFileP4_3);
//
//    vector<vector<unsigned char>> layer1PixelsP4;
//    vector<vector<unsigned char>> pattern1PixelsP4;
//    vector<vector<unsigned char>> pattern2PixelsP4;
//    vector<vector<unsigned char>> finalPixelsP4;
//
//    practice.storeData(readFileP4_1, layer1HeaderP4, layer1PixelsP4);
//    practice.storeData(readFileP4_2, pattern1HeaderP4, pattern1PixelsP4);
//    practice.storeData(readFileP4_3, pattern2HeaderP4, pattern2PixelsP4);
//
//    for(int i = 0; i < layer1PixelsP4.size(); i++) {
//        finalPixelsP4.push_back(practice.multiply(layer1PixelsP4.at(i), pattern1PixelsP4.at(i)));
//        finalPixelsP4.at(i) = practice.subtract(finalPixelsP4.at(i), pattern2PixelsP4.at(i));
//    }
//
//    practice.writeFile(copyFileP4,layer1HeaderP4,finalPixelsP4);
//
//    readFileP4_1.close();   readFileP4_2.close();   readFileP4_3.close();   copyFileP4.close();
//
///// Overlay test (Part5)
//
//    fstream readFileP5_1("./input/layer1.tga", ios_base::binary | ios_base::in);
//    fstream readFileP5_2("./input/pattern1.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP5("./output/part5.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP5 = practice.storeHeader(readFileP5_1);
//    Header pattern1HeaderP5 = practice.storeHeader(readFileP5_2);
//
//    vector<vector<unsigned char>> layer1PixelsP5;
//    vector<vector<unsigned char>> pattern1PixelsP5;
//    vector<vector<unsigned char>> finalPixelsP5;
//
//    practice.storeData(readFileP5_1, layer1HeaderP5, layer1PixelsP5);
//    practice.storeData(readFileP5_2, pattern1HeaderP5, pattern1PixelsP5);
//
//
//    for(int i = 0; i < layer1PixelsP5.size(); i++) {
//        finalPixelsP5.push_back(practice.overlay(layer1PixelsP5.at(i), pattern1PixelsP5.at(i)));
//    }
//
//    practice.writeFile(copyFileP5,layer1HeaderP5,finalPixelsP5);
//
//    readFileP5_1.close();   readFileP5_2.close();   copyFileP5.close();
//
///// Add green test (Part6)
//
//    fstream readFileP6_1("./input/car.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP6("./output/part6.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP6 = practice.storeHeader(readFileP6_1);
//
//    vector<vector<unsigned char>> layer1PixelsP6;
//    vector<vector<unsigned char>> finalPixelsP6;
//
//    practice.storeData(readFileP6_1, layer1HeaderP6, layer1PixelsP6);
//
//    for(int i = 0; i < layer1PixelsP6.size(); i++) {
//        finalPixelsP6.push_back(practice.add(layer1PixelsP6.at(i), {0, 200, 0}));
//    }
//
//    practice.writeFile(copyFileP6,layer1HeaderP6,finalPixelsP6);
//
//    readFileP6_1.close();   copyFileP6.close();
//
///// Multiply red by 4 and blue by 0 (Part7)
//
//    fstream readFileP7_1("./input/car.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP7("./output/part7.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP7 = practice.storeHeader(readFileP7_1);
//
//    vector<vector<unsigned char>> layer1PixelsP7;
//    vector<vector<unsigned char>> finalPixelsP7;
//
//    practice.storeData(readFileP7_1, layer1HeaderP7, layer1PixelsP7);
//
//    for(int i = 0; i < layer1PixelsP7.size(); i++) {
//        finalPixelsP7.push_back(practice.scale(layer1PixelsP7.at(i), {0,1,4}));
//    }
//
//    practice.writeFile(copyFileP7,layer1HeaderP7,finalPixelsP7);
//
//    readFileP7_1.close();   copyFileP7.close();
//
///// Separating channels (Part8)
//
//    fstream readFileP8_1("./input/car.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP8_b("./output/part8_b.tga", ios_base::binary | ios_base::out);
//    fstream copyFileP8_g("./output/part8_g.tga", ios_base::binary | ios_base::out);
//    fstream copyFileP8_r("./output/part8_r.tga", ios_base::binary | ios_base::out);
//
//    Header layer1HeaderP8 = practice.storeHeader(readFileP8_1);
//
//    vector<vector<unsigned char>> layer1PixelsP8;
//    vector<vector<unsigned char>> finalPixelsP8_b;
//    vector<vector<unsigned char>> finalPixelsP8_g;
//    vector<vector<unsigned char>> finalPixelsP8_r;
//
//
//    practice.storeData(readFileP8_1, layer1HeaderP8, layer1PixelsP8);
//
///// Blue
//    for(int i = 0; i < layer1PixelsP8.size(); i++) {
//        finalPixelsP8_b.push_back({layer1PixelsP8.at(i).at(0),layer1PixelsP8.at(i).at(0),layer1PixelsP8.at(i).at(0)});
//    }
//
///// Green
//    for(int i = 0; i < layer1PixelsP8.size(); i++) {
//        finalPixelsP8_g.push_back({layer1PixelsP8.at(i).at(1),layer1PixelsP8.at(i).at(1),layer1PixelsP8.at(i).at(1)});
//    }
//
///// Red
//    for(int i = 0; i < layer1PixelsP8.size(); i++) {
//        finalPixelsP8_r.push_back({layer1PixelsP8.at(i).at(2),layer1PixelsP8.at(i).at(2),layer1PixelsP8.at(i).at(2)});
//    }
//
//    practice.writeFile(copyFileP8_b, layer1HeaderP8, finalPixelsP8_b);
//    practice.writeFile(copyFileP8_g, layer1HeaderP8, finalPixelsP8_g);
//    practice.writeFile(copyFileP8_r, layer1HeaderP8, finalPixelsP8_r);
//
//    readFileP8_1.close();   copyFileP8_b.close();   copyFileP8_g.close();   copyFileP8_r.close();
//
//
///// Combining channels (Part9)
//
//    fstream layer1P9 ("./input/layer_blue.tga", ios_base::binary | ios_base::in);
//    fstream layer2P9 ("./input/layer_green.tga", ios_base::binary | ios_base::in);
//    fstream layer3P9 ("./input/layer_red.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP9 ("./output/part9.tga", ios_base::binary | ios_base::out);
//
//    vector<vector<unsigned char>> layer1PixelsP9;
//    vector<vector<unsigned char>> layer2PixelsP9;
//    vector<vector<unsigned char>> layer3PixelsP9;
//    vector<vector<unsigned char>> finalPixelsP9;
//
//    Header layer1HeaderP9 = practice.storeHeader(layer1P9);
//    Header layer2HeaderP9 = practice.storeHeader(layer2P9);
//    Header layer3HeaderP9 = practice.storeHeader(layer3P9);
//
//    practice.storeData(layer1P9,layer1HeaderP9,layer1PixelsP9);
//    practice.storeData(layer2P9,layer2HeaderP9,layer2PixelsP9);
//    practice.storeData(layer3P9,layer3HeaderP9,layer3PixelsP9);
//
//    for(int i = 0; i < layer1PixelsP9.size(); i++) {
//        finalPixelsP9.push_back({layer1PixelsP9.at(i).at(0), layer2PixelsP9.at(i).at(1), layer3PixelsP9.at(i).at(2)});
//    }
//
//    practice.writeFile(copyFileP9,layer1HeaderP9,finalPixelsP9);
//
//    layer1P9.close();   layer2P9.close();   layer3P9.close();   copyFileP9.close();
//
///// Rotating image (Part10)
//
//    fstream layer1P10 ("./input/text2.tga", ios_base::binary | ios_base::in);
//    fstream copyFileP10 ("./output/part10.tga", ios_base::binary | ios_base::out);
//
//    vector<vector<unsigned char>> layer1PixelsP10;
//    vector<vector<unsigned char>> finalPixelsP10;
//
//    Header layer1HeaderP10 = practice.storeHeader(layer1P10);
//
//    practice.storeData(layer1P10,layer1HeaderP10,layer1PixelsP10);
//    finalPixelsP10.resize(layer1PixelsP10.size());
//
//    int counter = layer1PixelsP10.size() - 1;
//    for(int i = 0; i < layer1PixelsP10.size(); i++) {
//        finalPixelsP10.at(counter) = layer1PixelsP10.at(i);
//        counter--;
//    }
//
//    practice.writeFile(copyFileP10,layer1HeaderP10,finalPixelsP10);
//
//    layer1P10.close();   copyFileP10.close();



    return 0;
}
