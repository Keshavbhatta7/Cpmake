#include <iostream>

#ifndef CPMAKE_H
#define CPMAKE_H


const std::string PROGRAM_NAME = "cpmake";
const std::string DEF_STRING_VAL = "NULL";
const std::string MAKEFILE_NAME = "Cpfile";
const int ERRCODE = -1;
const int DEF_SPACES = 10;

enum class Errcodes {
    INVALID_FILE_EXTENSION,
    INVALID_FLAG,
    INPUT_FILE_NOT_PROVIDED,
    FLAG_DOESNT_EXPECT_ARGS,
    FLAG_EXPECTS_ARGS,
    INVALID_SYNTAX,
    INVALID_FILE_NAME,
    FILE_DOESNT_EXIST,
    OCODE,
};

#define EXIT exit(ERRCODE)
#endif
