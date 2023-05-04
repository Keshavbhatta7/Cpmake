#include <iostream>
#include <string>

int main(void)
{
    std::string str;

    std::cout << "Enter string with '.': ";
    std::cin >> str;

    int dot_pos = str.find_last_of('.');
    str.erase(dot_pos);

    std::cout << "String without '.': " << str << std::endl;
    return 0;
}
