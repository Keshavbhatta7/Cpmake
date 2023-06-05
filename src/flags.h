#ifndef FLAGS_H
#define FLAGS_H

#include <iostream>
#include <vector>
#include "errhandle.h"
#include "cpmake.h"

class Main : public Cpmake {
public:
    void checkFlags(const int argc, std::vector<std::string>& args);
    void processFlags(std::vector<std::string>& args, size_t currPos);
};

#endif