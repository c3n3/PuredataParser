#define DEBUG 1
#include <iostream>
#include "PuredataFile.h"
#include <vector>
#include "Timer.h"

/**
 * @brief This is the main
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    Timer t("\nmain -> ");
    std::vector<PuredataFile*> files;
    if (argc == 1) 
    {
        std::cout << "You must input a file name(s) to command line args \n";
        return -1;
    }
    files.reserve(argc - 1);
    int offset = 0;
    for (int i = 1; i < argc; i++)
    {
#if DEBUG
        std::cout << "\n***********TOP FILE**************\nOFFSET: " << offset << " FILE: " << argv[i] << "\n\n"; 
#endif
        files.push_back(new PuredataFile(argv[i], offset));
        offset += files[i - 1]->objectCount;
    }
    PuredataFile::generateFile(files, "Master.pd");    
    return 0;
}