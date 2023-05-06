#include <iostream>
#include <string>

bool vector_pos = false;

int main(void)
{
    std::string str;
    std::cout << "Enter a string: ";
    std::cin >> str;

    if ( (str.find(".cpp")) != std::string::npos) {
        vector_pos = true;
    }

    std::cout << "Vector pos: " << vector_pos << std::endl;
    return 0;
}