#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#define PROGRAM_NAME "cpmake"
#define EXIT exit(0)
#define ERRCODE -1

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

    std::vector<std::string> extensions = {".cpp", ".c", ".cc", ".rs"};
    std::vector<std::string> compilers =  {"clang++", "clang", "clang++", "rustc"};

    size_t vector_size = extensions.size();
    int vector_pos = isvalid_file();
    
    int isvalid_file()
    {
        std::string extension;
        int dot_pos = input_file.find_last_of(".");

        if (dot_pos != std::string::npos) {
            extension = input_file.substr(dot_pos);
        }

        for (int i = 0; i < vector_size; i++) {
            if (extension == extensions[i]) return i;
        }

        return 0;
    }

    void printerr_messages(Errcodes errcode)
    {
        if (errcode == Errcodes::INVALID_FILE_EXTENSION) {
            std::cout << "error: invalid file extension" << std::endl;
            EXIT;
        }
    }

    std::string erase_dot(std::string str)
    {
        int dot_pos = str.find_last_of('.');
        str.erase(dot_pos-1);

        return str;
    }

    void set_output_file()
    {
        if (compilers[vector_pos] == "rustc") {
            output_file = "";
            return;
        }

        output_file = erase_dot(input_file);
        output_file += ".exe";
    }

    std::string compile_command()
    {
        set_output_file();
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
            compile_cmd += " -o ";
            compile_cmd += output_file;
        }

        return compile_cmd;
    }

    void compile()
    {
        
        if (vector_pos == ERRCODE) {
            printerr_messages(Errcodes::INVALID_FILE_EXTENSION);
        }

        std::string cmd = compile_command();

        std::cout << "________________compiling_________________" << std::endl;

        std::cout << cmd << std::endl;
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


