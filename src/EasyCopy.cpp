#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>

namespace fs = std::filesystem;

// Get global config path
fs::path getGlobalConfigPath() {
#ifdef _WIN32
    char* userProfile = std::getenv("USERPROFILE");
    return fs::path(userProfile) / "easycopy_config.txt"; // Windows
#else
    char* home = std::getenv("HOME");
    return fs::path(home) / ".easycopy_config.txt"; // Linux/macOS
#endif
}

// Load shortcuts from config (supports spaces using '|' delimiter)
std::map<std::string, std::pair<std::string, std::string>> loadShortcuts() {
    std::map<std::string, std::pair<std::string, std::string>> shortcuts;
    fs::path configPath = getGlobalConfigPath();

    if (!fs::exists(configPath)) return shortcuts;

    std::ifstream file(configPath);
    std::string line;
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

// Save shortcuts to config
void saveShortcuts(const std::map<std::string, std::pair<std::string, std::string>>& shortcuts) {
    fs::path configPath = getGlobalConfigPath();
    std::ofstream file(configPath, std::ios::trunc);
    for (auto& [name, paths] : shortcuts) {
        file << name << "|" << paths.first << "|" << paths.second << "\n";
    }
}

// Copy all files inside source to destination recursively
bool copyFolder(const fs::path& src, const fs::path& dst) {
    if (!fs::exists(src) || !fs::is_directory(src)) return false;
    if (!fs::exists(dst)) fs::create_directories(dst);

    for (auto& entry : fs::recursive_directory_iterator(src)) {
        const auto& path = entry.path();
        auto relative = fs::relative(path, src);
        fs::path destPath = dst / relative;

        if (fs::is_directory(path)) {
            fs::create_directories(destPath);
        } else {
            fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  ec copy <src> <dst>\n";
        std::cout << "  ec shortcut <name> copy <src> <dst>\n";
        std::cout << "  ec <shortcutName>\n";
        std::cout << "  ec delete <name>\n";
        std::cout << "  ec list\n";
        return 0;
    }

    std::map<std::string, std::pair<std::string, std::string>> shortcuts = loadShortcuts();
    std::string command = argv[1];

    if (command == "copy" && argc == 4) {
        fs::path src = argv[2];
        fs::path dst = argv[3];
        if (copyFolder(src, dst)) {
            std::cout << "Copied successfully.\n";
        } else {
            std::cout << "Failed to copy. Check source path.\n";
        }
    } 
    else if (command == "shortcut" && argc == 6 && std::string(argv[3]) == "copy") {
        std::string name = argv[2];
        std::string src = argv[4];
        std::string dst = argv[5];
        shortcuts[name] = {src, dst};
        saveShortcuts(shortcuts);
        std::cout << "Shortcut '" << name << "' saved.\n";
    } 
    else if (command == "delete" && argc == 3) {
        std::string name = argv[2];
        if (shortcuts.erase(name)) {
            saveShortcuts(shortcuts);
            std::cout << "Shortcut '" << name << "' deleted.\n";
        } else {
            std::cout << "Shortcut not found.\n";
        }
    } 
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
    else if (shortcuts.count(command)) {
        auto [src, dst] = shortcuts[command];
        if (copyFolder(src, dst)) {
            std::cout << "Copied successfully via shortcut '" << command << "'.\n";
        } else {
            std::cout << "Failed to copy. Check source path.\n";
        }
    } 
    else {
        std::cout << "Invalid command or shortcut not found.\n";
    }

    return 0;
}
