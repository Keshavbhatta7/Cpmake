#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "cpmake.h"

#ifndef FILEMP_H
    #define FILEMP_H

typedef struct {
    std::string compiler_flags;
    std::string input_files;
    std::string output_file;
} constants;

class FileMp {
public:
    constants fileMpInit(std::string filename);

    void setDefVals();
    void printVals(constants& consts);
    void printErs(Errcodes errcode);

    int fReadLine(std::string fname);
    int getCflags(std::string line_txt);
    int getFname(std::string line_txt);

    bool checkSyntax(std::vector<std::string>& syns, std::string txt);
};

#endif
