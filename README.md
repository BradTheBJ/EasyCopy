# EasyCopy

**EasyCopy** is a simple, cross-platform command-line tool for quickly copying directories and their contents. It’s useful for moving game saves, quick backups, or local file management.

## What it does
- Recursively copy all files and folders from a source directory to a destination.
- Save repeated copy tasks as named shortcuts.
- Run, list, or delete saved shortcuts with a single command.
- Validate that the source directory exists before copying to avoid errors.

## Important notes about this build
- Shortcuts are stored in a plain text config file located in the user's home:
  - Windows: %USERPROFILE%\easycopy_config.txt
  - Linux/macOS: $HOME/.easycopy_config.txt
- Each config line uses the pipe '|' as delimiter:
  name|source|destination
  - This allows spaces in paths. Do not include '|' in your paths.
- The program expects the source to be an existing directory. It copies the entire directory tree into the destination (creating destination directories as needed) and overwrites existing files.
- Creating, deleting, or updating shortcuts rewrites the global config file above. Manual edits are allowed but must follow the pipe-separated format exactly.

## Quick Usage

Run a saved shortcut by name:
```bash
ec <shortcutName>
```

Create or update a shortcut (saved to the global config):
```bash
ec shortcut <name> copy "<source>" "<destination>"
```

Delete a saved shortcut:
```bash
ec delete <name>
```

List saved shortcuts:
```bash
ec list
```

One-off copy (no shortcut saved):
```bash
ec copy "<source>" "<destination>"
```

Tips:
- Quote paths that contain spaces when typing commands in your shell.
- The tool validates that the source exists and is a directory before copying.
- Copying is recursive and will overwrite existing files at the destination.
- Avoid using the pipe character '|' inside paths because it is the config field separator.

To view the config file directly:
```powershell
# Windows
type "%USERPROFILE%\easycopy_config.txt"
```
```bash
# Linux/macOS
cat "$HOME/.easycopy_config.txt"
```

## Examples

One-off copy (Linux/macOS):
```bash
ec copy "/home/me/saves" "/mnt/backup/saves"
```

One-off copy (Windows PowerShell):
```powershell
ec copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"
```

Create a shortcut named `mygame`:
```bash
ec shortcut mygame copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"
```

Run the saved shortcut:
```bash
ec mygame
```

Delete the saved shortcut:
```bash
ec delete mygame
```

List shortcuts:
```bash
ec list
```

## Installation

Windows
1. Place `ec.exe` in a permanent folder, e.g. `C:\Tools\EasyCopy\`.
2. Add the folder to your PATH (PowerShell):
```powershell
setx PATH "$env:PATH;C:\Tools\EasyCopy"
```
3. Restart your terminal and run commands like:
```powershell
ec copy "C:\from" "D:\to"
```

Linux

Global (all users):
```bash
sudo mv "{downloaded_file}" /usr/local/bin/ec
sudo chmod +x /usr/local/bin/ec
```

Per-user:
```bash
mkdir -p "$HOME/tools"
mv "{downloaded_file}" "$HOME/tools/ec"
chmod +x "$HOME/tools/ec"
echo 'export PATH="$PATH:$HOME/tools"' >> ~/.bashrc
source ~/.bashrc
```

macOS

Build and install system-wide:
```bash
xcode-select --install
clang++ -std=c++17 src/EasyCopy.cpp -o ec
sudo mv ec /usr/local/bin/ec
sudo chmod +x /usr/local/bin/ec
```

Per-user:
```bash
mkdir -p "$HOME/Tools/EasyCopy"
mv ec "$HOME/Tools/EasyCopy/ec"
chmod +x "$HOME/Tools/EasyCopy/ec"
echo 'export PATH="$PATH:$HOME/Tools/EasyCopy"' >> ~/.zshrc
source ~/.zshrc
```

32-bit machines: compile locally with:
```bash
g++ -std=c++17 -m32 src/EasyCopy.cpp -o ec
```

## Compiling

Requires a C++17 compiler with <filesystem> support.

Windows (Clang or MinGW):
```bash
clang++ -std=c++17 src/EasyCopy.cpp -o ec.exe
# or
g++ -std=c++17 src/EasyCopy.cpp -o ec.exe
```

macOS:
```bash
clang++ -std=c++17 src/EasyCopy.cpp -o ec
```

Linux:
```bash
g++ -std=c++17 src/EasyCopy.cpp -o ec
```

On Debian/Ubuntu, if needed:
```bash
sudo apt install build-essential
```

## Troubleshooting
- Ensure the source directory exists and is readable.
- Quote any paths with spaces when invoking ec.
- Make sure the destination is writable (or use a destination you can create).
- The program reads/writes the global config in your home directory (see Important notes) — use `ec list` or open the config file to inspect saved shortcuts.
- For large or complex backups, consider a dedicated backup tool.

## License
EasyCopy is licensed under GPL-3.0. See the LICENSE file.