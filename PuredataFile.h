#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include "buffer_functions.h"
using namespace std::string_literals;

/**
 * @brief The pure data file
 * 
 */
class PuredataFile
{
public:
    char* objects;
    char* connects;
    int objectCount;

    int inputObject;
    int outputObject;
    void toString();
    PuredataFile(const char * name, int offset);
    int offset(int startIndex);
    void restringConnects(const std::string& objectNumber, const std::string& replace);
    void cleanConnects();
    void printConnects();
    static void generateFile(std::vector<PuredataFile*>& files, const char* name);
};

/**
 * @brief This pairs all together
 * 
 * @param files 
 * @param name 
 */
void PuredataFile::generateFile(std::vector<PuredataFile*>& files, const char* name) {
    std::ofstream out(name);
    out << "#N canvas 2521 143 995 666 12;\n";

    // add all the objects to the file
    int startIndex = 0;
    // std::cout << files[0].objects << "\n";
    for (int i = 0; i < files.size(); i++) {
        // std::cout << "\n\nBEFORE";
        // files[i].printConnects();  
        // startIndex = files[i]->offset(startIndex);
        // std::cout << "\n\nAfter";
        // files[i].printConnects();
        // files[i]->toString();
        startIndex += files[i]->objectCount;
        out << files[i]->objects << "\n";
    }

    // create adc and dac
    out << "#X obj 0 0 adc~;\n";
    out << "#X obj 0 0 dac~;\n";

    out << "#X msg 378 115 \n; pd dsp 1 \n;;\n#X obj 376 53 loadbang;\n";

    // create the adc connection and dac
    int adcIndex = startIndex;
    out << "#X connect " << adcIndex << " 0 " << files[0]->inputObject << " 0;\n";
    out << "#X connect " << adcIndex << " 1 " << files[0]->inputObject << " 0;\n";
    int dacIndex = startIndex + 1;
    out << "#X connect " << files[files.size() - 1]->outputObject << " 0 " << dacIndex << " 0;\n";
    out << "#X connect " << files[files.size() - 1]->outputObject << " 0 " << dacIndex << " 1;\n";
    out << "#X connect " << startIndex + 3 << " 0 " << startIndex + 2 << " 0;\n";

    // input all the standard connects
    for (int i = 0; i < files.size(); i++) {
        out << files[i]->connects << "\n";
    }

    // create the connections between objects
    for (int i = 1; i < files.size(); i++) {
        out << "#X connect " << files[i - 1]->outputObject << " 0 " << files[i]->inputObject << " 0;\n";
    }
    out.close();
}

/**
 * @brief Construct a new Puredata File:: Puredata File object
 * 
 * @param name of file
 * @param offset the starting object number that all must 
 */
PuredataFile::PuredataFile(const char *name, int offset)
{
    Timer t("File Time -> ");
    FILE *f;
    f = fopen(name, "r");// // read mode
#if DEBUG
    std::cout << "File: " << f << " name: " << name << "\n";
#endif
    if (f == NULL)
    {
        return;
    }
    int c;
    int bufLen = 100;
    char* buf = (char*)malloc(sizeof(char) * bufLen);
    int conBufLen = 1000;
    int objBufLen = 1000;

    // the index
    int oi = 0;
    
    objects = (char*)calloc(objBufLen, sizeof(char));

    //the index
    int ci = 0;
    connects = (char*)calloc(conBufLen, sizeof(char));

    if (!connects || !objects) {
        std::cout << "\n\n*********************************FAIL DUE TO CALLOC****************************";
        std::cout << "\n*********************************FAIL DUE TO CALLOC****************************";
        std::cout << "\n*********************************FAIL DUE TO CALLOC****************************\n\n";
    }

    outputObject = 0;
    inputObject = 0;
    objectCount = 0;
    int index = -1; // the start of each file has a header so we start at -1
    bool onConnects = false;
    for (int i = 0; (c = fgetc(f)) != EOF; i++)
    {
        buf[i] = c;
        if (c == ';')
        {
            buf[i + 1] = fgetc(f); // there is a new line after the semi colon
            buf[i + 2] = 0;
            i = -1;

            if (onConnects || (buf[3] == 'c' && buf[4] == 'o'))
            {
                onConnects = true;
                int i = 0;
                int spaces = 0;
                for (;offset != 0 && spaces < 5; i++) {
                    if (buf[i] == ' ') {
                        spaces++;
                    }
                    if (spaces == 2 || spaces == 5) {
                        spaces++;
                        offsetNumberWithNumber(&buf, offset, i + 1, bufLen);
                    }
                }
                concat(&connects, ci, conBufLen, buf);
                ci += strlen(buf);
            }
            else if (buf[1] == 'X')
            {
                objectCount++;
                concat(&objects, oi, objBufLen, buf);
                oi += strlen(buf);
            }
            if (!onConnects && ( buf[23] == '1' && buf[18] == '1' && buf[19] == '.' && buf[20] == '0' && buf[21] == '0' && buf[22] == '0'))
            {
                inputObject = index + offset;
            }
            else if (!onConnects && (buf[23] == '2' && buf[18] == '1' && buf[19] == '.' && buf[20] == '0' && buf[21] == '0' && buf[22] == '0'))
            {
                outputObject = index + offset;
            }
            index++;
        }
    }
    fclose(f);
#if DEBUG
    std::cout << "object BUF usage: " << strlen(objects) << " / " << objBufLen << " = " <<(strlen(objects)* 1.0) / objBufLen << "\n";
    std::cout << "connects BUF usage: " << strlen(connects) << " / " << conBufLen << " = " <<(strlen(connects)* 1.0) / conBufLen << "\n";
#endif
}

/**
 * @brief print file
 * 
 */
void PuredataFile::toString()
{
    std::cout << "********************************OBJECTS***********************************\n";
    std::cout << objects << "\n";
    std::cout << "********************************CONNECTS***********************************\n";
    std::cout << connects << "\n";
}
