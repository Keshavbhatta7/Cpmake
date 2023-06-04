#include <iostream>
#include <filesystem>
#include "flags.h"
#include "cpmake.h"
#include "errhandle.h"

void Flags::processFlags(std::vector<std::string>& args, size_t pos, Cpmake& cpmake)
{
    size_t argsLen = args.size();

    if (args[pos] == "-o") {
        int _ = cpmake.setVectorPos();
        if (_ == ERRCODE) cpmake.checkIF();

        cpmake.definedVectorPos = true;
        cpmake.setOutputFile(args[pos+1]);

        cpmake.definedOutputFile = true;
    } else if (args[pos] == "-h") {
       cpmake.checkNxtArgs(args, pos, argsLen, Errcodes::FLAG_DOESNT_EXPECT_ARGS);
       cpmake.printUsage();
       EXIT;
    } else if (args[pos] == "-m") {
        cpmake.checkIF();
        cpmake.setVectorPos();
        cpmake.setOutputFile();
        cpmake.compilerFlags = args[pos+1];
        cpmake.setCompileCmd();
    } else if (args[pos] == "-r") {
        cpmake.runOutputFile = true;
        size_t size = args.size();
        if (pos == size && cpmake.inputFile == DEF_STRING_VAL) {
                printErrs(Errcodes::FLAG_EXPECTS_ARGS);
            }
    } else if (args[pos] == "-k") {
        cpmake.deleteOutputFile = true;
    }
}

void Flags::checkFlags(const int argc, std::vector<std::string>& args, Cpmake& cpmake)
{
    for (int i = 1; i < argc; i++) {
        processFlags(args, i, cpmake);
    }
}