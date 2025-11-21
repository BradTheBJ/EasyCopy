#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // Open config file in append mode (for saving shortcuts)
    std::ofstream outfile("easycopy_config.cpp", std::ios::app);

    // Variables
    bool shortcut = false;
    std::string optionalCommand1, optionalCommand2, command, source, destination, extraArg;
    int i = 1;

    // Mode 1: Execute shortcut if only one argument is provided
    if (argc == 2) {
        std::string shortcutName = argv[1];
        std::ifstream infile("easycopy_config.cpp");
        std::string line;

        while (std::getline(infile, line)) {
            std::size_t pos = line.find(shortcutName + " = \"");
            if (pos != std::string::npos) {
                std::size_t end = line.find("\";", pos + shortcutName.length() + 4);
                if (end != std::string::npos) {
                    std::string value = line.substr(pos + shortcutName.length() + 4, end - pos - shortcutName.length() - 4);

                    // Parse the saved copy command: "copy "<source>" "<destination>" [extraArg]"
                    size_t cmdPos = value.find("copy ");
                    if (cmdPos == std::string::npos) {
                        std::cerr << "Shortcut command format invalid.\n";
                        return 1;
                    }

                    // Extract source
                    size_t srcStart = value.find('"', cmdPos) + 1;
                    size_t srcEnd = value.find('"', srcStart);
                    std::string execSource = value.substr(srcStart, srcEnd - srcStart);

                    // Extract destination
                    size_t dstStart = value.find('"', srcEnd + 1) + 1;
                    size_t dstEnd = value.find('"', dstStart);
                    std::string execDestination = value.substr(dstStart, dstEnd - dstStart);

                    std::cout << "Executing shortcut: copy " << execSource << " -> " << execDestination << std::endl;

                    // Use std::filesystem::copy
                    fs::path srcPath = execSource;
                    fs::path dstPath = execDestination;

                    if (!fs::exists(srcPath)) {
                        std::cerr << "Source path does not exist: " << srcPath << "\n";
                        return 1;
                    }

                    try {
                        fs::copy(srcPath, dstPath, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
                        std::cout << "Copy completed.\n";
                    } catch (fs::filesystem_error& e) {
                        std::cerr << "Filesystem error: " << e.what() << "\n";
                    }

                    return 0;
                }
            }
        }

        std::cerr << "Shortcut '" << shortcutName << "' not found.\n";
        return 1;
    }

    // Mode 2: Optional commands + copy
    if (argc < 4 || argc > 7) {
        std::cerr << "Usage: " << argv[0]
                  << " [optional_command1] [optional_command2_if_shortcut] copy source destination [optionalArg]\n";
        return 1;
    }

    // Check first optional command
    if (std::string(argv[i]) != "copy") {
        optionalCommand1 = argv[i++];
        if (optionalCommand1 == "shortcut") {
            shortcut = true;
        }
    }

    // Check second optional command if first was shortcut
    if (shortcut) {
        if (i >= argc || std::string(argv[i]) == "copy") {
            std::cerr << "Error: You must provide a name for the shortcut as a second command.\n";
            return 1;
        }
        optionalCommand2 = argv[i++];
    } else if (i < argc && std::string(argv[i]) != "copy") {
        optionalCommand2 = argv[i++];
    }

    // Next argument must be copy
    if (i >= argc) {
        std::cerr << "Error: Missing 'copy' command.\n";
        return 1;
    }
    command = argv[i++];

    // Source and destination
    if (i + 1 >= argc) {
        std::cerr << "Error: Missing source or destination.\n";
        return 1;
    }
    source = argv[i++];
    destination = argv[i++];
    if (argc > i) extraArg = argv[i];

    // Validate source path
    fs::path srcPath = source;
    fs::path dstPath = destination;
    if (!fs::exists(srcPath)) {
        std::cerr << "Error: Source path does not exist: " << source << "\n";
        return 1;
    }

    // Handle shortcut creation
    if (shortcut) {
        // Save paths in quotes to preserve spaces/apostrophes
        outfile << optionalCommand2 << " = \""
                << command << " "
                << "\"" << source << "\" "
                << "\"" << destination << "\"";
        if (!extraArg.empty())
            outfile << " " << extraArg;
        outfile << "\";\n";

        std::cout << "Shortcut '" << optionalCommand2 << "' saved in config.\n";
    }

    // Perform copy
    if (command == "copy") {
        try {
            fs::copy(srcPath, dstPath, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            std::cout << "Copy completed.\n";
        } catch (fs::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << "\n";
        }
    }

    return 0;
}
