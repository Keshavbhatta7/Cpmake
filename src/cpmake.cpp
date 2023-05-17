#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <windows.h>
#include "fileio.h"

/* What is vector_pos variabe?
* It is the most important variable in this whole code
* After checking the file extension of the input_file
* We set vector_pos to the postition extension of the input_file in vector
* [extensions] and as the position of compilers in [compilers] vector
* is set relative to the extensions in [extensions] vector it
* also tells the compiler needed to compile current file and a lot other.
* So, It is very very important.
*/


enum class Errcodes {
    INVALID_FILE_EXTENSION,
    INVALID_FLAG,
    INPUT_FILE_NOT_PROVIDED,
    FLAG_DOESNT_EXPECT_ARGS,
    FLAG_EXPECTS_ARGS,
    OCODE,
};

typedef enum class Errcodes Errcodes;
Errcodes errcodes;

class Cpmake {
private:
    std::string compiler = DEF_STRING_VAL;
    std::string input_file = DEF_STRING_VAL;
    std::string output_file = DEF_STRING_VAL;
    std::string compile_cmd = DEF_STRING_VAL;

    std::vector<std::string> extensions = {".cpp", ".c", ".cc", ".rs"};
    std::vector<std::string> compilers =  {"clang++", "clang", "clang++", "rustc"};
    std::vector<std::string> flags = {"-o", "-h", "-m", "-r"};

    bool defined_output_file;
    bool defined_vector_pos;
    bool run_afterwards;

    int vector_size = extensions.size();
    int flags_size = flags.size();
    int vector_pos {};

public:

    int print_smth(size_t times, char ch, bool newline = false, size_t newline_times = 1)
    {
        for (int i = 1; i <= times; i++) {
            std::cout << ch;
        }

        if (newline) {
            for (int i = 1; i <= newline_times; i++) {
                std::cout << '\n';
            }
        }
        return EXIT_SUCCESS;
    }
    
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
        } else if (errcode == Errcodes::FLAG_DOESNT_EXPECT_ARGS) {
            std::cerr << "warning: provided flag doesn't expect any arguments" << std::endl;
            return;
        } else if (errcode == Errcodes::FLAG_EXPECTS_ARGS) {
            std::cerr << "warning: provided flags expects more argument";
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

    int compile()
    {
        if (compile_cmd == DEF_STRING_VAL) set_compile_cmd();
        int len = compile_cmd.size();


        print_smth(len+DEF_SPACES, '_', true, 2);
        print_smth(DEF_SPACES/2, ' ');

        std::cout << compile_cmd << std::endl;
        std::system(compile_cmd.c_str());

        print_smth(len+DEF_SPACES, '_', true);
        return EXIT_SUCCESS;
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
        /* for (int i = 0; i < flags_size; i++) {
            if (strs[curr_pos] == flags[i] || strs[curr_pos-1] == flags[i])
            return;
        } */

        for (int i = 0; i < flags_size; i++) {
            if (strs[curr_pos] == flags[i]) return;
            else if (curr_pos > 0) {
                if  (strs[curr_pos-1] == "-o") return;
            }
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

            // check_input_file();
            int ret_val = set_vector_pos();
            if (ret_val == ERRCODE) check_input_file();

            defined_vector_pos = true;
            set_output_file(strs[curr_pos+1]);

            defined_output_file = true;
        } else if (strs[curr_pos] == "-h") {
            check_args_after_flags(strs, curr_pos, strs_len, Errcodes::FLAG_DOESNT_EXPECT_ARGS);
            print_usage();
            EXIT;
        } else if (strs[curr_pos] == "-m") {
            check_input_file();
            set_vector_pos();
            set_output_file();
            set_compile_cmd(strs[curr_pos+1]);
        } else if (strs[curr_pos] == "-r") {
            run_afterwards = true;
            size_t size = strs.size();
            if (curr_pos == size && input_file == DEF_STRING_VAL) {
                printerr_messages(Errcodes::FLAG_EXPECTS_ARGS);
            }
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
        check_input_file();
        if (!defined_vector_pos) set_vector_pos();
        if (!defined_output_file) set_output_file();
    }

    int get_term_cols()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);

        return csbi.srWindow.Right - csbi.srWindow.Left+1;
    }

    int run_out()
    {
        if (run_afterwards) {
            check_input_file();
            int spaces = get_term_cols();
            std::cout <<  "running " << output_file << std::endl;

            system(output_file.c_str());
            std::cout << std::endl;

            return EXIT_SUCCESS;
        }

        return EXIT_FAILURE;
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
        /* freadline("sample.txt");
        std::cout << std::endl; */
        cpmake.print_usage();
    }

    cpmake.set_input_file(argc, args);
    cpmake.check_flags(argc, args);

    cpmake.isdefined();
    cpmake.compile();
    cpmake.run_out();

    return 0;
}
