#ifndef FILEMP_H
#define FILEMP_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

struct constants {
    std::string compiler;
    std::string compilerFlags;
    std::string inputFiles;
    std::string outputFile;
};

typedef struct constants constants;

class FileMp {
public:
    constants fileMpInit(std::string filename);

    void setDefVals();
    void printVals(constants& consts);

    int fReadLine(std::string fname);
    int getCflags(std::string str);
    int getFileNames(std::string str);
    int getOutName(std::string str);
    int getCompiler(std::string str);
    int getEnd(size_t quote1, size_t quote2);

    bool checkSyntax(std::vector<std::string>& syns, std::string str);
    bool checkConsts(constants& consts);
};

#endif
