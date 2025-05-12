#include "../include/main_utils.h"

#include <iostream>

// Parse command line arguments.
void parse_command_line(int argc, char* argv[], std::string& dir_path) {
    int opt;
    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
            case 'd':
                dir_path = optarg;
                break;
            case '?':
                std::cout << usage << std::endl;
                break;
        }
    }
}

// Append all the file names in 'dir_path' in 'filenames'.
void get_all_filenames(const std::string& dir_path, std::vector<std::string>& filenames) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(dir_path.c_str())) != NULL) {
        // process all the files insider the directory
        while ((ent = readdir (dir)) != NULL) {
            std::string file_name = ent->d_name;
            // Don't consider the current directory '.' and the parent ".."
            if (file_name == "." || file_name == "..") {
                continue;
            }
            if (*(dir_path.end() - 1) == '/') {
                filenames.push_back(dir_path + file_name);
            } else {
                filenames.push_back(dir_path + "/" + file_name);
            }
        }
        closedir(dir); // Close the directory.
    }
}
