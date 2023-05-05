#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#define PROGRAM_NAME "cpmake"
#define EXIT exit(0)
#define ERRCODE -1

#define SUCCESS_CODE 7

enum class Errcodes {
    INVALID_FILE_EXTENSION,
    INVALID_FLAG,
    FILE_DOESNT_EXIST,
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
    std::vector<std::string> flags = {"-o"};

    size_t vector_size = extensions.size();
    size_t vector_pos;
    
    size_t isvalid_file(std::string input_file)
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
        } else if (errcode == Errcodes::FILE_DOESNT_EXIST) {
            std::cout << "error: file doesn't exist" << std::endl;
            EXIT;
        } else if (errcode == Errcodes::INVALID_FLAG) {
            std::cout << "error: invalid flag" << std::endl;
        }
    }

    std::string erase_dot(std::string str)
    {
        size_t dot_pos = str.find_last_of('.');
        str.erase(dot_pos);

        return str;
    }

    void set_output_file(std::string out_file = "")
    {
        if (compilers[vector_pos] == "rustc") {
            output_file = "";
            return;
        }

        if (out_file == "") {
            output_file = erase_dot(input_file);
            output_file += ".exe";
        } else {
            output_file = out_file;
        }
    }

    std::string compile_command()
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
            compile_cmd += " -o ";
            compile_cmd += output_file;
        }

        return compile_cmd;
    }

    void compile()
    {
        std::string cmd = compile_command();
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

    int try_file_types(std::string file_name)
    {
        for (int i = 0; i < vector_size; i++) {
            std::string temp_file = file_name;
            temp_file += extensions[i];

            std::filesystem::path file_path("./" + temp_file);
            if  (std::filesystem::exists(file_path)) {
                input_file = temp_file;
                return SUCCESS_CODE;
            } else printerr_messages(Errcodes::FILE_DOESNT_EXIST);
        }

        return ERRCODE;
    }

    int does_file_exists(std::string file_name)
    {
       std::filesystem::path file_path(file_name);

       if (std::filesystem::exists(file_path)) {
            input_file = file_name;
            return SUCCESS_CODE;
        } else {
            printerr_messages(Errcodes::FILE_DOESNT_EXIST);
        }

        return ERRCODE;
    }

    int binary_search(std::vector<std::string>& vec, std::string key)
    {
        int s = 0;
        int e = vec.size()-1;

        while (s <= e) {
            int mid = (s+e)/2;

            if (vec[mid] == key) return mid;
            else if (vec[mid] > key) e = mid-1;
            else s = mid+1;
        }

        return ERRCODE;
    }

    void flag_O(int curr_flag_pos, std::vector<std::string>& flags)
    {
        std::string temp_output_file = flags[curr_flag_pos+1];
        if (temp_output_file == "") {
            set_output_file(temp_output_file);
        }
        else if ( (temp_output_file.find(".exe")) == std::string::npos) {
            temp_output_file += ".exe";
            output_file = temp_output_file;
        }

    }

    void valid_flags(std::string flag, std::vector<std::string>& flags)
    {
        int O = binary_search(flags, "-o");
        if (O != ERRCODE) flag_O(O, flags);
        else printerr_messages(Errcodes::INVALID_FLAG);
    }

    int manager(std::string str, std::vector<std::string>& args)
    {
        int ret_value {};

        if ( (str.find('-')) != std::string::npos) valid_flags(str, args);
        else if ( (isvalid_file(str)) != ERRCODE ) { 
            ret_value = does_file_exists(str); 
        }
        else {
            ret_value = try_file_types(str);
        }

        return SUCCESS_CODE;
    }

    void set_input_file(int argc, char* argv[])
    {
        std::vector<std::string> args(argv, argc + argv);
        std::string temp_input_file = "_";

        int ret_value {};

        for (int i = 1; i < argc; i++) {
            ret_value = manager(args[i], args);
            if (ret_value == SUCCESS_CODE) vector_pos = i-1;  // i-1 as loop is from 1
        }

    }
};

int main(int argc, char* argv[])
{
    Cpmake cpmake;
    if (argc < 2) cpmake.print_usage();
    cpmake.set_input_file(argc, argv);
    cpmake.compile();

    std::cout << cpmake.vector_pos << std::endl;
    std::cout << cpmake.input_file << std::endl;
    std::cout << cpmake.output_file << std::endl;
    std::cout << cpmake.def_compiler << std::endl;
    std::cout << cpmake.compile_cmd << std::endl;
}


/* Features (Options):
 1. -o flag eg cpmake file.cpp -o file. output_file = file + ".exe" :/
 */
