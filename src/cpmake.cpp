#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#define PROGRAM_NAME "cpmake"
#define EXIT exit(0)

#define ERRCODE -1

enum class Errcodes {
    INVALID_FILE_EXTENSION,
    INVALID_FLAG,
    FILE_DOESNT_EXIST,
    OCODE,
};

typedef enum class Errcodes Errcodes;
Errcodes errcodes;

class Cpmake {
public:
    std::string compiler;
    std::string input_file;
    std::string output_file;
    std::string compile_cmd;

    std::vector<std::string> extensions = {".cpp", ".c", ".cc", ".rs"};
    std::vector<std::string> compilers =  {"clang++", "clang", "clang++", "rustc"};
    std::vector<std::string> flags = {"-o"};

    bool defined_output_file;

    int vector_size = extensions.size();
    int flags_size = flags.size();
    int vector_pos {};
    
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
        int dot_pos = str.find_last_of('.');
        str.erase(dot_pos);

        return str;
    }

    void set_output_file(std::string out_file = "null")  // out_file = examp
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

    std::string get_compile_cmd()
    {
        compiler = compilers[vector_pos];

        if (compiler == "rustc") {
            compile_cmd = compiler;
            compile_cmd += " ";
            compile_cmd += input_file;
        }

        else {
            compile_cmd = compiler;
            compile_cmd += " ";
            compile_cmd += input_file;
            compile_cmd += " -o ";
            compile_cmd += output_file;
        }

        return compile_cmd;
    }

    void compile()
    {
        std::string cmd = get_compile_cmd();
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

    void try_file_types(std::string curr_arg)
    {
        for (int i = 0; i < vector_size; i++) {
            std::string temp_arg = curr_arg;
            temp_arg += extensions[i];

            std::filesystem::path file_path(temp_arg);
            if  (std::filesystem::exists(file_path)) {
                input_file = temp_arg;
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
        }

    }

    void manager(std::string curr_arg)
    {
        if ( (isvalid_file(curr_arg)) != ERRCODE ) does_file_exists(curr_arg); 
        else try_file_types(curr_arg);

    }

    void set_vector_pos()
    {
        for (int i = 0; i < vector_size; i++) {
            if ((input_file.find(extensions[i])) != std::string::npos) {
                vector_pos = i;
            } else continue;
        }
    }

    void set_input_file(int argc, char* argv[])
    {
        std::vector<std::string> args(argv, argc + argv);
        input_file = "_";
        args.push_back(" ");

        for (int i = 1; i < argc; i++) {
            manager(args[i]);
            if (input_file != "_") break;
        }

        set_vector_pos();
    }

    void do_things(std::string arg, std::string next_arg) {
        if (arg == "-o") {
            set_output_file(next_arg);
            defined_output_file = true;
        }
    }

    void check_flags(const int argc, char* argvals[])
    {
        std::vector<std::string> args(argvals, argvals + argc);

        for (size_t i = 0; i < argc; i++) {
            for (size_t j = 0; j < flags_size; j++) {
                if (args[i] == flags[j]) do_things(args[i], args[i+1]);
            }
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
};

int main(int argc, char* argv[])
{
    Cpmake cpmake;
    if (argc < 2) cpmake.print_usage();
    cpmake.set_input_file(argc, argv);
    cpmake.check_flags(argc, argv);
    if (!cpmake.defined_output_file) cpmake.set_output_file();
    cpmake.compile();
    
    cpmake.print_everything();

    return 0;
}