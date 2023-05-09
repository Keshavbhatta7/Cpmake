#include <iostream>
#include <fstream>
#include <string>

int main(void)
{
    std::string file_contents;

    std::ifstream inf {"sample.txt"};
    std::string the_ting;

    // Syntax: cflags="clear"
    if (inf.is_open()) {

        while (std::getline(inf, file_contents)) {
            int t1 = file_contents.find("cflags=");
            if (t1 == std::string::npos) t1 = file_contents.find("cflags = ");
            if (t1 == std::string::npos) continue;

            if ( (std::count(file_contents.begin(), file_contents.end(), '"')) > 2) {
                std::cerr << "you can't have more than two double quotes" << std::endl;
                continue;
            }

            int pos1 = file_contents.find_first_of('"');
            int pos2 = file_contents.find_last_of('"');

            the_ting = file_contents.substr(pos1, pos2);
            std::cout << the_ting << std::endl;
        }

    } else {
        std::cerr << "couldn't open the file." << std::endl;
    }

    return 0;
}