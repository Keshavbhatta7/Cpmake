#include <fileio.h>

int freadline(std::string fname)
{
    std::ifstream inf { fname.c_str() };
    std::string line_content;

    if (!inf.is_open()) return ERRCODE;
    while (std::getline(inf, line_content)) {
        line_content += "\n";
        std::cout << line_content << std::endl;
    }
}