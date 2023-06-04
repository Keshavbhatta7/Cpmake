#ifndef FLAGS_H
#define FLAGS_H

#include <iostream>
#include <vector>
#include "errhandle.h"
#include "cpmake.h"

class Flags {
public:
    void checkFlags(const int argc, std::vector<std::string>& args, Cpmake& cpmake);
    void processFlags(std::vector<std::string>& args, size_t currPos, Cpmake& cpmake);
};

#endif