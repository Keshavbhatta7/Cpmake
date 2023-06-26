#ifndef FLAGS__H
#define FLAGS__H

#include <iostream>
#include <vector>

#include "errhandle.h"
#include "cpmake.h"

class Main : public Cpmake {
public:
    void checkFlags(const int argc, std::vector<std::string>& args);
    void processFlags(std::vector<std::string>& args, const size_t currPos);

    bool isItFlag(std::vector<std::string>& args, const size_t pos);
    void flagO(std::vector<std::string>& args, const size_t pos);
    void flagH(std::vector<std::string>& args, const size_t pos, const size_t argsLen);
    void flagM(std::vector<std::string>& args, const size_t pos);
    void flagR(std::vector<std::string>& args, const size_t pos);
    void flagC(std::vector<std::string>& args, const size_t pos, const size_t argsLen);
    void flagF(std::vector<std::string>& args, const size_t pos);

    std::string translateFlags(std::string flag);
};

#endif