# EasyCopy

**EasyCopy** is a simple, cross-platform command-line tool for quickly copying files. It’s designed to help anyone who needs to move files between folders efficiently—whether for game saves, backups, or general file management.

## What it does
- Copy all files inside a folder to another location.  
- Save repeated copy tasks as shortcuts.  
- Run or delete saved shortcuts with a single command.  
- Check that source files exist before copying to avoid errors.  

**Note:** Currently, EasyCopy copies the files inside a folder but does **not** create the parent folder at the destination. Support for full directory copy will be added in the future.

## Features
- Recursive file copying within directories  
- Creation and persistence of named shortcuts  
- Shortcut execution and deletion by name  
- Source path validation prior to copying  
- Shortcuts stored and editable in `easycopy_config.cpp`  

## Quick Usage
Copy files from one folder to another:
```bash
ec copy "<source>" "<destination>"
```
> Tips:  
> - Quote paths that contain spaces.  
> - Copying is recursive and will overwrite existing files automatically.

## Shortcuts
**Create a shortcut**:
```bash
ec shortcut <name> copy "<source>" "<destination>"
```

**Run a shortcut**:
```bash
ec <shortcutName>
```

**Delete a shortcut**:
```bash
ec delete <name>
```

> Once created, shortcuts can be run anytime without typing the full source and destination.

## Installation

### Windows
1. Move the downloaded file to a permanent folder, e.g., `C:\Tools\EasyCopy\`.
2. Rename it to `ec.exe` for consistency.
3. Add the folder to your PATH:
```powershell
setx PATH "$env:PATH;C:\Tools\EasyCopy"
```
4. Restart your terminal and run:
```powershell
ec copy "C:\from" "D:\to"
```

### Linux
**Global installation (all users)**:
```bash
sudo mv "{downloaded_file}" /usr/local/bin/ec
sudo chmod +x /usr/local/bin/ec
```
**Per-user installation**:
```bash
mkdir -p "$HOME/tools"
mv "{downloaded_file}" "$HOME/tools/ec"
chmod +x "$HOME/tools/ec"
echo 'export PATH="$PATH:$HOME/tools"' >> ~/.bashrc
source ~/.bashrc
```

### macOS
Prebuilt binary is **not provided**, compile locally:
```bash
xcode-select --install
clang++ -std=c++17 src/EasyCopy.cpp -o ec
sudo mv ec /usr/local/bin/ec
sudo chmod +x /usr/local/bin/ec
```
Or per-user:
```bash
mkdir -p "$HOME/Tools/EasyCopy"
mv ec "$HOME/Tools/EasyCopy/ec"
chmod +x "$HOME/Tools/EasyCopy/ec"
echo 'export PATH="$PATH:$HOME/Tools/EasyCopy"' >> ~/.zshrc
source ~/.zshrc
```

**32-bit machines:** 64-bit files won’t run. Compile locally with:
```bash
g++ -std=c++17 -m32 src/EasyCopy.cpp -o ec
```

## Examples
```bash
# One-off copy
ec copy "/home/me/saves" "/mnt/backup/saves"
ec copy "C:\Games\BG3\Saves" "D:\Backup\BG3"

# Create a shortcut
ec shortcut bg3 copy "C:\Games\BG3\Saves" "D:\Backup\BG3"

# Run a saved shortcut
ec bg3

# Delete a saved shortcut
ec delete bg3
```

## Cheat Sheet
```bash
# Run saved shortcut
ec <name>

# Delete a saved shortcut
ec delete <name>

# Create a copy shortcut
ec shortcut <name> copy <src> <dst>

# One-off copy
ec copy "<src>" "<dst>"
```

## Troubleshooting
- Ensure the source folder exists.  
- Quote any paths with spaces.  
- Make sure the destination folder is writable.  
- EasyCopy reads shortcuts from the current folder; run it from the correct location or use full paths.  
- For large backups, consider a dedicated backup tool.  

## License
EasyCopy is licensed under **GPL-3.0**. See the [LICENSE](LICENSE) file
