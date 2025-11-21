# EasyCopy

EasyCopy is a small cross-platform command-line tool to quickly copy files or directories (useful for game saves and simple backups). It's lightweight, easy to compile, and simple to modify.

Quick recommendation: add the `ec` executable to your system PATH so you can run it from anywhere.

---

## Features

- Copy files or entire directories (recursive)
- Create named shortcuts for frequent copy operations
- Execute or delete saved shortcuts by name
- Validate source paths before copying
- Shortcuts stored in a simple config file (`easycopy_config.cpp`)
- Cross-platform: Windows, macOS, Linux

---

## Usage

General syntax:

- Run a saved shortcut:
```bash
ec <shortcutName>
```

- Create a shortcut:
```bash
ec shortcut <name> copy "<source>" "<destination>"
```

- Delete a shortcut:
```bash
ec delete <name>
```

- One-off copy (no shortcut):
```bash
ec copy "<source>" "<destination>"
```

Notes:
- Quote any path containing spaces.
- Copying is recursive and overwrites existing files.
- Shortcuts are appended to and read from `easycopy_config.cpp`. Manual edits are allowed but must follow the same formatting the program uses.

---

## Examples

Run saved shortcut:
```bash
ec bg3
```

Create a shortcut named `bg3`:
```bash
ec shortcut bg3 copy "C:\Games\BG3\Saves" "D:\Backup\BG3"
```

Delete the `bg3` shortcut:
```bash
ec delete bg3
```

One-off copy:
```bash
ec copy "/home/me/saves" "/mnt/backup/saves"
```

---

## Add `ec` to PATH (recommended)

Windows (PowerShell)
1. Put `ec.exe` in a permanent folder, e.g. `C:\Tools\EasyCopy`.
2. Run (adds to user PATH):
```powershell
setx PATH "$env:PATH;C:\Tools\EasyCopy"
```
3. Restart PowerShell/Command Prompt.

macOS
Option 1 — system location:
```bash
sudo mv ec /usr/local/bin/
sudo chmod +x /usr/local/bin/ec
```

Option 2 — custom folder:
```bash
mkdir -p ~/Tools/EasyCopy
mv ec ~/Tools/EasyCopy/
# Add to shell profile (e.g. ~/.zshrc):
echo 'export PATH="$PATH:$HOME/Tools/EasyCopy"' >> ~/.zshrc
source ~/.zshrc
```

Linux
Option 1 — system location:
```bash
sudo mv ec /usr/local/bin/
sudo chmod +x /usr/local/bin/ec
```

Option 2 — custom folder:
```bash
mkdir -p ~/tools
mv ec ~/tools/
# Add to ~/.bashrc:
echo 'export PATH="$PATH:$HOME/tools"' >> ~/.bashrc
source ~/.bashrc
```

---

## Compiling

Requires a C++17 compiler with filesystem support.

Windows (Clang or MinGW):
```bash
clang++ -std=c++17 EasyCopy.cpp -o ec.exe
# or
g++ -std=c++17 EasyCopy.cpp -o ec.exe
```

macOS:
```bash
clang++ -std=c++17 EasyCopy.cpp -o ec
```

Linux:
```bash
g++ -std=c++17 EasyCopy.cpp -o ec
```

On Debian/Ubuntu, install build tools if needed:
```bash
sudo apt install build-essential
```

---

## Notes & Troubleshooting

- If a shortcut fails, verify the source path exists and permissions are sufficient.
- Manual edits to `easycopy_config.cpp` are accepted but must match the program's expected format.
- If multiple copies are running, be cautious about simultaneous writes to the same destination.
- For large transfers consider using a dedicated backup tool; EasyCopy is intended for simple, local copy tasks.

---