#include <iostream>
#include <string>
#include <vector>

#define ERRCODE -1

int isvalid_file(std::vector<std::string> extensions, std::string input_file, 
        size_t vector_size)
{
    std::string extension;
    int dot_pos = input_file.find_last_of('.');
    if (dot_pos != std::string::npos) {
        extension = input_file.substr(dot_pos);
    }

    for (int i = 0; i < vector_size; i++) {
        if (extension == extensions[i]) return i;
    }

    return ERRCODE;
}

int main()
{
    std::vector<std::string> extensions = {".cpp", ".rs", ".c", ".cc"};
    std::string input_file;
    size_t vector_size = extensions.size();

    std::cout << "Enter input_file: ";
    std::cin >> input_file;

    if ((isvalid_file(extensions, input_file, vector_size)) != std::string::npos) {
        std::cout << "YESSIR!!" << std::endl;
    } else {
        std::cout << "NOSIR!!" << std::endl;
    }
}
