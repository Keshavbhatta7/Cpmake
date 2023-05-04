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
    
    size_t isvalid_file()
    {
        std::string extension;
        size_t dot_pos = input_file.find_last_of(".");

        if (dot_pos != std::string::npos) {
            extension = input_file.substr(dot_pos);
        }

        for (size_t i = 0; i < vector_size; i++) {
            if (extension == extensions[i]) {
                return i;
            }
        }

        return std::string::npos;
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
        size_t dot_pos = str.find_last_of('.');
        str.erase(dot_pos);

        return str;
    }

    void set_output_file(size_t vector_pos)
    {
        if (compilers[vector_pos] == "rustc") {
            output_file = "";
            return;
        }

        output_file = erase_dot(input_file);
        output_file += ".exe";
    }

    std::string compile_command(size_t vector_pos)
    {


        set_output_file(vector_pos);
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
        size_t vector_pos = isvalid_file();

        if (vector_pos == std::string::npos) {
            printerr_messages(Errcodes::INVALID_FILE_EXTENSION);
        }

        std::string cmd = compile_command(vector_pos);

        std::cout << "_____________________________________________" << "\n\n";

        std::cout << "    " << cmd << std::endl;
        std::system(cmd.c_str());

        std::cout  << "_____________________________________________" << '\n';
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


