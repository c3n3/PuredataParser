#include <iostream>
#include "PuredataFile.h"
#include <vector>
#include "Timer.h"

int main(int argc, char *argv[])
{
    Timer t("main -> ");
    std::vector<PuredataFile*> files;
    if (argc == 1) 
    {
        std::cout << "You must input a file name(s) to command line args \n";
        return -1;
    }
    files.reserve(argc - 1);
    for (int i = 1; i < argc; i++)
    {
        files.push_back(new PuredataFile(argv[i]));
    }
    PuredataFile::generateFile(files, "Master.pd");    
    return 0;
}