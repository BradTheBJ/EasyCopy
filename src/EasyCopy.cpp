#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>

namespace fs = std::filesystem;

// Get global config file path depending on OS (Windows or Unix-like)
fs::path getGlobalConfigPath() {
#ifdef _WIN32
    char* userProfile = std::getenv("USERPROFILE");
    return fs::path(userProfile) / "easycopy_config.txt"; // Windows config in user profile
#else
    char* home = std::getenv("HOME");
    return fs::path(home) / ".easycopy_config.txt"; // Linux/macOS hidden config
#endif
}

// Load saved shortcuts from the config file (format: name|src|dst)
std::map<std::string, std::pair<std::string, std::string>> loadShortcuts() {
    std::map<std::string, std::pair<std::string, std::string>> shortcuts;
    fs::path configPath = getGlobalConfigPath();

    if (!fs::exists(configPath)) return shortcuts;

    std::ifstream file(configPath);
    std::string line;

    // Each line is parsed into name, src, dst
    while (std::getline(file, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');

        if (pos1 != std::string::npos && pos2 != pos1) {
            std::string name = line.substr(0, pos1);
            std::string src = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string dst = line.substr(pos2 + 1);
            shortcuts[name] = {src, dst};
        }
    }
    return shortcuts;
}

// Overwrite config file with all current shortcuts
void saveShortcuts(const std::map<std::string, std::pair<std::string, std::string>>& shortcuts) {
    fs::path configPath = getGlobalConfigPath();
    std::ofstream file(configPath, std::ios::trunc);

    for (auto& [name, paths] : shortcuts) {
        file << name << "|" << paths.first << "|" << paths.second << "\n";
    }
}

// Recursively copy source folder into destination folder
bool copyFolder(const fs::path& src, const fs::path& dst) {
    std::string response;

    // Validate source exists and is a folder
    if (!fs::exists(src) || !fs::is_directory(src))
        return false;

    // Create destination folder automatically if missing
    if (!fs::exists(dst))
        fs::create_directories(dst);

    // Prompt user for confirmation regardless of empty/non-empty
    if (fs::is_empty(src)) {
        std::cout << "Source folder is empty. Proceed with copy? (y/n): ";
    } else {
        std::cout << "Source folder is not empty. Proceed with copy? (y/n): ";
    }
    std::cin >> response;
    if (!(response == "y" || response == "Y"))
        return false;

    // Perform recursive file copy
    for (auto& entry : fs::recursive_directory_iterator(src)) {
        const auto& path = entry.path();
        auto relative = fs::relative(path, src);
        fs::path destPath = dst / relative;

        // Create directories or copy files
        if (fs::is_directory(path)) {
            fs::create_directories(destPath);
        } else {
            fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    // Basic command usage guide
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  ec copy <src> <dst>\n";
        std::cout << "  ec shortcut <name> copy <src> <dst>\n";
        std::cout << "  ec <shortcutName>\n";
        std::cout << "  ec delete <name>\n";
        std::cout << "  ec list\n";
        return 0;
    }

    // Load all shortcuts from config
    std::map<std::string, std::pair<std::string, std::string>> shortcuts = loadShortcuts();
    std::string command = argv[1];

    // Direct copy command
    if (command == "copy" && argc == 4) {
        fs::path src = argv[2];
        fs::path dst = argv[3];

        if (copyFolder(src, dst)) {
            std::cout << "Copied successfully.\n";
        } else {
            std::cout << "Failed to copy. Check source path or user canceled.\n";
        }
    }

    // Create a new shortcut
    else if (command == "shortcut" && argc == 6 && std::string(argv[3]) == "copy") {
        std::string name = argv[2];
        std::string src = argv[4];
        std::string dst = argv[5];

        shortcuts[name] = {src, dst};
        saveShortcuts(shortcuts);

        std::cout << "Shortcut '" << name << "' saved.\n";
    }

    // Delete an existing shortcut
    else if (command == "delete" && argc == 3) {
        std::string name = argv[2];

        if (shortcuts.erase(name)) {
            saveShortcuts(shortcuts);
            std::cout << "Shortcut '" << name << "' deleted.\n";
        } else {
            std::cout << "Shortcut not found.\n";
        }
    }

    // List all shortcuts
    else if (command == "list") {
        if (shortcuts.empty()) {
            std::cout << "No shortcuts saved.\n";
        } else {
            std::cout << "Saved shortcuts:\n";
            for (auto& [name, paths] : shortcuts) {
                std::cout << "  " << name << ": " << paths.first << " -> " << paths.second << "\n";
            }
        }
    }

    // Use a shortcut if command matches its name
    else if (shortcuts.count(command)) {
        auto [src, dst] = shortcuts[command];

        if (copyFolder(src, dst)) {
            std::cout << "Copied successfully via shortcut '" << command << "'.\n";
        } else {
            std::cout << "Failed to copy. Check source path or user canceled.\n";
        }
    }

    // Unknown command or shortcut
    else {
        std::cout << "Invalid command or shortcut not found.\n";
    }

    return 0;
}
