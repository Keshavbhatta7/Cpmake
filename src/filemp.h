#include <iostream>
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


void setvals();
void printers(Errcodes errcode);
int freadline(std::string fname);

constants filemp_init(std::string filename);
std::string getcflags(std::string line_txt);
std::string getfiles(std::string line_txt);



#endif
