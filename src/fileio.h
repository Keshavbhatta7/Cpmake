#include <iostream>
#include <fstream>
#include <string>

#ifndef FILEIO_H
    #define FILEIO_H


#define PROGRAM_NAME "cpmake"
#define EXIT exit(0)
#define DEF_STRING_VAL "NULL"
#define ERRCODE -1
#define DEF_SPACES 10

int freadline(std::string fname);

#endif