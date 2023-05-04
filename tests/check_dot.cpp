#include <iostream>
#include <string>

int main()
{
    std::string str;

    std::cout << "Enter some string: ";
    std::cin >> str;

    int isthere_ext = str.find(".cpp");

    if (isthere_ext != std::string::npos) {
        std::cout << "there is .cpp in the str" << std::endl;
    } else {
        std::cout << "there isn't .cpp in the str" << std::endl;
    }
}
