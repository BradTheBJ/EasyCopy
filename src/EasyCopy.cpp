#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc == 4) {
        std::string arg = argv[1];
        std::string arg2 = argv[2];
        std::string arg3 = argv[3];

        if (arg == "copy") {
            fs::path source = arg2;
            fs::path destination = arg3;

            try {
                fs::copy(source, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            }
            catch (fs::filesystem_error& e) {
                std::cerr << "Filesystem error: " << e.what() << "\n";
            }
        }
    } else {
        std::cout << "No arguments provided.\n";
    }
}
