#include <iostream>

#include "errhandle.h"
#include "cpmake.h"

void printErrs(Errcodes errcode)
{
    switch (errcode)
    {
        case Errcodes::INVALID_FILE_NAME: std::cerr << "error: file name should be 'Cpfile'" << std::endl;
        EXIT;

        case Errcodes::INVALID_SYNTAX: std::cerr << "error: invalid syntax" << std::endl;
        EXIT;

        case Errcodes::FILE_DOESNT_EXIST: std::cerr << "error: file doesn't exist" << std::endl;
        EXIT;

        case Errcodes::INPUT_FILE_NOT_PROVIDED: std::cerr << "error: input file isn't provided" << std::endl;
        EXIT;

        case Errcodes::COMPILER_NOT_PROVIDED: std::cerr << "error: compiler is not defined in 'Cpfile'" << std::endl;
        EXIT;

        case Errcodes::OUTPUT_FILE_NOT_PROVIDED: std::cerr << "error: output file isn't provided" << std::endl;
        EXIT;

        case Errcodes::INVALID_FILE_EXTENSION: std::cerr << "error: invalid file extension" << std::endl;
        EXIT;

        case Errcodes::INVALID_FLAG: std::cerr << "error: invalid flag" << std::endl;
        EXIT;

        case Errcodes::FLAG_DOESNT_EXPECT_ARGS: std::cerr << "error: flag doesn't expect any arguments" << std::endl;
        EXIT;

        case Errcodes::FLAG_EXPECTS_ARGS: std::cerr << "error: flags expects arguments" << std::endl;
        break;
    }
}