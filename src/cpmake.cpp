#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#define PROGRAM_NAME "cpmake"
#define EXIT exit(0)
const int ERRCODE = -1;

enum class Errcodes {
    INVALID_FILE_EXTENSION,
};

Errcodes errcodes;

class Cpmake {
public:
    std::string def_compiler;
    std::string input_file;
    std::string output_file;
    std::string compile_cmd;

    std::vector<std::string> extensions = { ".cpp", ".c", ".cc", ".rs" };
    std::vector<std::string> compilers =  { "clang++", "clang", "clang++", "rustc" };

    size_t exten_vec_size = extensions.size();
    int isvalid_file()
    {
        for (int i = 0; i < exten_vec_size; i++) {
            if ( (input_file.find(extensions[i]) ) != std::string::npos) {
                return i;
            }
        }

        return ERRCODE;
    }

    void printerr_messages(Errcodes errcode)
    {
        if (errcode == Errcodes::INVALID_FILE_EXTENSION) {
            std::cout << "error: invalid file extension" << std::endl;
            EXIT;
        }
    }

    std::string compile_command(int vector_pos)
    {
        def_compiler = compilers[vector_pos];
        if (def_compiler == "rustc") {
            compile_cmd = def_compiler;
            compile_cmd += " ";
            compile_cmd += input_file;
        }

        else {
            compile_cmd = def_compiler;
            compile_cmd += " ";
            compile_cmd += input_file;
            compile_cmd += " ";
            compile_cmd += output_file;
        }

        return compile_cmd;
    }

    void compile()
    {
        int vector_pos = isvalid_file();
        
        if (vector_pos == ERRCODE) {
            printerr_messages(Errcodes::INVALID_FILE_EXTENSION);
        }

        std::string cmd = compile_command(vector_pos);
        std::cout << "________________compiling_________________" << std::endl;
        std::system(cmd.c_str());
        std::cout << "________________compiling_________________" << std::endl;
    }

    void print_usage()
    {
        std::cout << "Usage: " << PROGRAM_NAME << " [input_file]" << std::endl;
        EXIT;
    }

};

int main(int argc, char* argv[])
{
    Cpmake cpmake;
    if (argc < 2) cpmake.print_usage();

    cpmake.input_file = argv[1];
    cpmake.compile();
}


