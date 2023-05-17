#include <iostream>
#include <fstream>
#include <string>

int main()
{
    const std::string filename = "./sample.txt";
    std::string fline_content;
    std::fstream inf { filename };

    if (!inf) {
        std::cerr << "couldn't open " << filename << std::endl;
        return 0;
    }

    while (std::getline(inf, fline_content)) {
        std::cout << fline_content << std::endl;
    }

    return 0;
}