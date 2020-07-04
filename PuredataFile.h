#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
using namespace std::string_literals;

struct PdObject {
    std::string line;
    int number;
    PdObject(const char* line, int number): line(line), number(number) {
    }
};

class PuredataFile
{
public:
    char* objects;
    char* connects;
    int objectCount;

    int inputObject;
    int outputObject;
    void toString();
    PuredataFile(const char * name);
    int offset(int startIndex);
    void restringConnects(const std::string& objectNumber, const std::string& replace);
    void cleanConnects();
    void printConnects();
    static void generateFile(std::vector<PuredataFile*>& files, const char* name);
};

void PuredataFile::generateFile(std::vector<PuredataFile*>& files, const char* name) {
    std::ofstream out(name);
    out << "#N canvas 2521 143 995 666 12;\n";

    // add all the objects to the file
    int startIndex = 0;
    // std::cout << files[0].objects << "\n";
    for (int i = 0; i < files.size(); i++) {
        // std::cout << "\n\nBEFORE";
        // files[i].printConnects();  
        startIndex = files[i]->offset(startIndex);
        // std::cout << "\n\nAfter";
        // files[i].printConnects();

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

    // input all the standar connects
    for (int i = 0; i < files.size(); i++) {
        out << files[i]->connects << "\n";
    }

    // create the connections between objects
    for (int i = 1; i < files.size(); i++) {
        out << "#X connect " << files[i - 1]->outputObject << " 0 " << files[i]->inputObject << " 0;\n";
    }
    out.close();
}

int PuredataFile::offset(int startIndex) {
    if (startIndex == 0) return objectCount;
    inputObject += startIndex;
    outputObject += startIndex;
    for (int i = 0; i < objectCount; i++) {
        restringConnects(std::to_string(i), std::to_string(i + startIndex));
    }
    cleanConnects();
    return startIndex + objectCount;
}

void PuredataFile::printConnects() {
    std::cout << "\n\n ******************** Connnects below: ********************************\n";
    std::cout << connects << "\n";
}

void PuredataFile::cleanConnects() {
    std::regex r("\\[}", std::regex_constants::optimize);
    connects = std::regex_replace(connects, r, "");
}

void PuredataFile::restringConnects(const std::string& objectNumber, const std::string& replace) {
    std::regex r("((t )"s + objectNumber + "( )|( )" + objectNumber + "( \\d;))"s, std::regex_constants::optimize);
    std::string rp = "$2$4[}"s + replace + "$3$5"s;
    connects = std::regex_replace(connects, r, rp);
}

PuredataFile::PuredataFile(const char *name)
{
    FILE *f;
    f = fopen(name, "r");// // read mode
    // std::cout << "File: " << f << " name: " << name << "\n";
    if (f == NULL)
    {
        return;
    }
    char c;
    char buf[100];
    int conBufLen = 5000;
    int objBufLen = 5000;

    // the index
    int oi = 0;
    objects = (char*)malloc(sizeof(char) * objBufLen);

    //the index
    int ci = 0;
    connects = (char*)malloc(sizeof(char) * conBufLen);


    outputObject = 0;
    inputObject = 0;
    objectCount = 0;
    std::regex obj("#X");
    std::regex connect("#X connect");
    std::regex out("1.0002");
    std::regex in("1.0001");
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
            if (onConnects || std::regex_search(buf, connect))
            {
                onConnects = true;
                concat(&connects, ci++, conBufLen, buf);
            }
            else if (std::regex_search(buf, obj))
            {
                concat(&connects, ci++, conBufLen, buf);
                objectCount++;
                objects += buf;
            }
            if (!onConnects && std::regex_search(buf, in))
            {
                inputObject = index;
            }
            else if (!onConnects && std::regex_search(buf, out))
            {
                outputObject = index;
            }
            index++;
        }
    }
    fclose(f);
}

void PuredataFile::toString()
{
    std::cout << objects << "\n";
    std::cout << connects << "\n";
}
