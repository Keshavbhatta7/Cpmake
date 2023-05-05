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

    bool output_file_choosed;

    int vector_size = extensions.size();
    int vector_pos;
    
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
                out_file =+ ".exe";
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

    void try_file_types(std::string file_name)
    {
        for (int i = 0; i < vector_size; i++) {
            std::string temp_file = file_name;
            temp_file += extensions[i];

            std::filesystem::path file_path("./" + temp_file);
            if  (std::filesystem::exists(file_path)) {
                input_file = temp_file;
                return;
            }
        }

        printerr_messages(Errcodes::FILE_DOESNT_EXIST);
    }

    void does_file_exists(std::string file_name)
    {
       std::filesystem::path file_path(file_name);

       if (std::filesystem::exists(file_path)) {
            input_file = file_name;
        }
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

    void manager(std::string str, std::vector<std::string>& args)
    {
        if ( (isvalid_file(str)) != ERRCODE ) does_file_exists(str); 
        else try_file_types(str);

    }

    void set_vector_pos()
    {
        vector_pos = 0;
        for (int i = 0; i < vector_size; i++) {
            if ((input_file.find(extensions[i])) != std::string::npos) {
                vector_pos = i;
            }
        }
    }

    void set_input_file(int argc, char* argv[])
    {
        std::vector<std::string> args(argv, argc + argv);
        input_file = "_";

        for (int i = 1; i < argc; i++) {
            manager(args[i], args);
            if (input_file != "_") return;
        }

        set_vector_pos();
    }

    void do_things(std::vector<std::string>& strs, size_t t, size_t pos) {
        print_everything();
        if (flags[t] == "-o") {
            set_output_file(strs[pos+1]);
            output_file_choosed = true;
        }
    }

    void check_flags(const int argc, char* argvals[])
    {
        std::vector<std::string> args(argvals, argvals + argc);
        const size_t len = flags.size();

        for (size_t i = 0; i < argc; i++) {
            for (size_t j = 0; j < len; j++) {
                if (args[i] == flags[j]) do_things(args, j, i);
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
    if (!cpmake.output_file_choosed) cpmake.set_output_file();
    cpmake.compile();

    return 0;
}