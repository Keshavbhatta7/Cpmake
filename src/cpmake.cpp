#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fileio.h>


enum class Errcodes {
    INVALID_FILE_EXTENSION,
    INVALID_FLAG,
    INPUT_FILE_NOT_PROVIDED,
    _H_DOESNT_EXPECT_ANY_ARGUMENTS,
    OCODE,
};

typedef enum class Errcodes Errcodes;
Errcodes errcodes;

class Cpmake {
private:
    std::string compiler;
    std::string input_file = DEF_STRING_VAL;
    std::string output_file;
    std::string compile_cmd = DEF_STRING_VAL;

    std::vector<std::string> extensions = {".cpp", ".c", ".cc", ".rs"};
    std::vector<std::string> compilers =  {"clang++", "clang", "clang++", "rustc"};
    std::vector<std::string> flags = {"-o", "-h", "-m"};

    bool defined_output_file;
    bool defined_vector_pos;

    int vector_size = extensions.size();
    int flags_size = flags.size();
    int vector_pos {};

public:
    
    int isvalid_file(std::string input_file)
    {
        std::string extension;
        int dot_pos = input_file.find_last_of(".");

        if (dot_pos != std::string::npos) {
            extension = input_file.substr(dot_pos);
        }

        for (int i = 0; i < vector_size; i++) {
            if (extension == extensions[i]) {
                return i;
            }
        }

        return ERRCODE;
    }


    void printerr_messages(Errcodes errcode)
    {
        if (errcode == Errcodes::INVALID_FILE_EXTENSION) {
            std::cerr << "error: invalid file extension" << std::endl;
            EXIT;
        } else if (errcode == Errcodes::INPUT_FILE_NOT_PROVIDED) {
            std::cerr << "error: program excepts an input file" << std::endl;
            EXIT;
        } else if (errcode == Errcodes::INVALID_FLAG) {
            std::cerr << "error: invalid flag" << std::endl;
            EXIT;
        } else if (errcode == Errcodes::_H_DOESNT_EXPECT_ANY_ARGUMENTS) {
            std::cerr << "Note: '-h' flag doesn't expect any arguments" << std::endl;
            return;
        }
    }

    std::string erase_dot(std::string str)
    {
        int dot_pos = str.find_last_of('.');
        str.erase(dot_pos);

        return str;
    }

    void set_output_file(std::string out_file = "null")
    {
        if (compilers[vector_pos] == "rustc") {
            output_file = "";
        }

        if (out_file == "null" || out_file == "" || out_file == " ") {
            std::string temp_out_file = erase_dot(input_file);
            temp_out_file += ".exe";
            output_file = temp_out_file;
        } else {
            int ret_value = out_file.find(".exe");
            if (ret_value == std::string::npos) {
                out_file += ".exe";
                output_file = out_file;
            }
        }

    }

    void set_compile_cmd(std::string extra = DEF_STRING_VAL)
    {
        compiler = compilers[vector_pos];

        if (compiler == "rustc") {
            compile_cmd = compiler;
            compile_cmd += " ";
            compile_cmd += input_file;
            if (extra != DEF_STRING_VAL) {
                compile_cmd += " ";
                compile_cmd += extra;
            }
        }

        else {
            compile_cmd = compiler;
            compile_cmd += " ";
            compile_cmd += input_file;
            compile_cmd += " -o ";
            compile_cmd += output_file;

            if (extra != DEF_STRING_VAL) {
                compile_cmd += " ";
                compile_cmd += extra;
            }
        }
    }

    void compile()
    {
        if (compile_cmd == DEF_STRING_VAL) set_compile_cmd();
        std::cout << "_____________________________________________" << "\n\n";

        std::cout << "    " << compile_cmd << std::endl;
        std::system(compile_cmd.c_str());

        std::cout  << "_____________________________________________" << '\n';
    }

    void print_usage()
    {
        std::cout << "Usage: " << PROGRAM_NAME << 
        " [input_file] [-o  (output_file)]" <<
        " [-m (compiler_flags)]" << std::endl;
        EXIT;
    }

    void try_file_types(std::string curr_arg)
    {
        for (int i = 0; i < vector_size; i++) {
            std::string temp_arg = curr_arg;
            temp_arg += extensions[i];

            std::filesystem::path file_path(temp_arg);
            if  (std::filesystem::exists(file_path)) {
                input_file = temp_arg;
                std::cout << "Note: using '" << input_file << "' as input file!!" << std::endl;
                return;
            }
        }

        return;
    }

    void does_file_exists(std::string str)
    {
       std::filesystem::path file_path("./" + str);
       if (std::filesystem::exists(file_path)) {
            input_file = str;
            return;
        }

        printerr_messages(Errcodes::INVALID_FILE_EXTENSION);

    }

    int search(std::vector<std::string>& strs, std::string find)
    {
        int strt = 0;
        int end = strs.size()-1;

        while (strt <= end) {
            int mid = (strt+end)/2;

            if (strs[mid] == find) return mid;
            else if (strs[mid] > find) end = mid-1;
            else strt = mid+1;
        }

        return ERRCODE;
    }

    void manager(std::vector<std::string>& strs, size_t curr_pos)
    {
        if (strs[curr_pos] == " " || strs[curr_pos] == "") return;
        for (int i = 0; i < flags_size; i++) {
            if (strs[curr_pos] == flags[i] || strs[curr_pos-1] == flags[i])
            return;
        }

        if ( (isvalid_file(strs[curr_pos]) ) != ERRCODE )
            does_file_exists(strs[curr_pos]);
        else try_file_types(strs[curr_pos]);
    }

    int set_vector_pos()
    {
        for (int i = 0; i < vector_size; i++) {
            if ((input_file.find(extensions[i])) != std::string::npos) {
                vector_pos = i;
                return 0;
            }
        }

        return ERRCODE;
    }

    void set_input_file(int argc, std::vector<std::string>& args)
    {
        for (int i = 1; i < argc; i++) {
            manager(args, i);
            if (input_file != DEF_STRING_VAL) {
                break;
            }
        }
    }

    void check_args_after_flags(std::vector<std::string>& strs, size_t curr_pos, size_t len, Errcodes errcode)
    {
        for (int i = curr_pos+1; i < len; i++) {
            if (strs[i] != " " && strs[i] != "") printerr_messages(errcode);
        }
    }


    // Checks the args and if there is any flags, it executes the function of those flags
    void exec_flags(std::vector<std::string>& strs, size_t curr_pos) {
        size_t strs_len = strs.size();

        if (strs[curr_pos] == "-o") {

            // Check if input file is provided or not.
            check_input_file();
            int ret_val = set_vector_pos();
            defined_vector_pos = true;
            if (ret_val == ERRCODE) check_input_file();

            set_output_file(strs[curr_pos+1]);
            defined_output_file = true;
        } else if (strs[curr_pos] == "-h") {
            check_args_after_flags(strs, curr_pos, strs_len, Errcodes::_H_DOESNT_EXPECT_ANY_ARGUMENTS);
            print_usage();
            EXIT;
        } else if (strs[curr_pos] == "-m") {
            check_input_file();
            set_vector_pos();
            set_output_file();
            set_compile_cmd(strs[curr_pos+1]);
        }
    }

    void check_flags(const int argc, std::vector<std::string>& args)
    {
        for (int i = 1; i < argc; i++) {
            exec_flags(args, i);
        }
    }

    void print_everything()
    {
        std::cout << input_file << std::endl;
        std::cout << output_file << std::endl;
        std::cout << compiler << std::endl;
        std::cout << compile_cmd << std::endl;

        std::cout << vector_size << std::endl;
        std::cout << vector_pos << std::endl;
    }

    void check_input_file()
    {
        if (input_file == DEF_STRING_VAL) 
            printerr_messages(Errcodes::INPUT_FILE_NOT_PROVIDED);
    }

    void isdefined()
    {
        if (!defined_vector_pos) set_vector_pos();
        if (!defined_output_file) set_output_file();
    }
};

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    // Is important for functins that run til args.size()+1;
    args.push_back(" ");

    Cpmake cpmake;
    if (argc < 2) {
        freadline("sample.txt");
        std::cout << std::endl;
        cpmake.print_usage();
    }

    cpmake.set_input_file(argc, args);
    cpmake.check_flags(argc, args);

    cpmake.isdefined();
    cpmake.compile();

    return 0;
}