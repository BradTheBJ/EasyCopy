# EasyCopy

EasyCopy is a small, cross-platform command-line utility for quickly copying directories and their contents. It's intended for simple local transfers (game saves, quick backups). Shortcuts are stored in a minimal plain-text config file.

See implementation: `src/EasyCopy.cpp`.

---

## Highlights

- Recursively copy a source directory into a destination (creates destination dirs, overwrites files).
- Save, run, list, and delete named shortcuts.
- Global config stored in your home directory.
- Validates source exists and is a directory before copying.

---

## Quick commands

Run a saved shortcut:
```bash
ec <shortcutName>
```

One-off copy:
```bash
ec copy "<source>" "<destination>"
```

Create/update shortcut:
```bash
ec shortcut <name> copy "<source>" "<destination>"
```

Delete shortcut:
```bash
ec delete <name>
```

List shortcuts:
```bash
ec list
```

---

## Config file & format

- Windows: `%USERPROFILE%\easycopy_config.txt`  
- Linux/macOS: `$HOME/.easycopy_config.txt`

Each line:
```
name|source|destination
```
The pipe `|` separates fields; do not include `|` in paths. Use `ec list` to view shortcuts.

---

## Examples

Linux/macOS one-off:
```bash
ec copy "/home/me/saves" "/mnt/backup/saves"
```

Windows PowerShell one-off:
```powershell
ec copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"
```

Create, run, delete:
```bash
ec shortcut mygame copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"
ec mygame
ec delete mygame
ec list
```

---

## Installation (recommended: prebuilt binary)

Windows
1. Download the 64‑bit `ec.exe`.
2. Place it in a permanent folder, e.g. `C:\Tools\EasyCopy\`.
3. Rename to `ec.exe` for consistent commands:
```powershell
Rename-Item "DownloadedName.exe" "ec.exe"
```
4. Add folder to PATH (PowerShell):
```powershell
setx PATH "$env:PATH;C:\Tools\EasyCopy"
# then open a new shell
```

macOS / Linux
1. Rename the downloaded file to `ec` and move to a PATH directory:
```bash
mv downloaded-file ec
chmod +x ec
sudo mv ec /usr/local/bin/
```
2. Verify:
```bash
which ec
ec --help
```

Note: prebuilt 32‑bit binaries are provided only for Linux. Windows and macOS prebuilt releases are 64‑bit only. If you need a 32‑bit binary on those platforms, build locally.

---

## Local compilation

Requirements: C++17 compiler with <filesystem> support (GCC 9+, recent Clang, or MSVC 2017+).

Linux / macOS:
```bash
g++ -std=c++17 src/EasyCopy.cpp -o ec
# or
clang++ -std=c++17 src/EasyCopy.cpp -o ec
```
If <filesystem> errors occur with older GCC:
```bash
g++ -std=c++17 src/EasyCopy.cpp -lstdc++fs -o ec
```

Windows (MinGW / MSYS / clang):
```bash
g++ -std=c++17 src/EasyCopy.cpp -o ec.exe
```

32‑bit Linux example (requires multilib):
```bash
g++ -std=c++17 -m32 src/EasyCopy.cpp -o ec32
```

After build, rename to `ec`/`ec.exe` and move to a PATH location.

---

## Adding EasyCopy to PATH (summary)

Windows (PowerShell):
```powershell
setx PATH "$env:PATH;C:\Tools\EasyCopy"
# open a new terminal
where.exe ec
```

macOS / Linux (per-user):
```bash
mkdir -p "$HOME/tools"
mv ec "$HOME/tools/ec"
chmod +x "$HOME/tools/ec"
echo 'export PATH="$PATH:$HOME/tools"' >> ~/.bashrc   # or ~/.zshrc
source ~/.bashrc
which ec
```

System-wide (Unix):
```bash
sudo mv ec /usr/local/bin/
sudo chmod +x /usr/local/bin/ec
which ec
```

---

## 32‑bit compatibility

- Only Linux receives prebuilt 32‑bit binaries.
- To build 32‑bit on Linux: install multilib packages (e.g. `gcc-multilib`) and compile with `-m32`.
- For Windows 32‑bit, use a 32‑bit MinGW toolchain.
- Modern macOS (10.15+) does not support 32‑bit user apps.

---

## Troubleshooting (actionable)

Failed copy — check source:
```bash
ls -la "/path/to/source"
# PowerShell:
Get-ChildItem "C:\path\to\source"
```
Permissions — ensure read access to source and write access to destination.

Shortcuts missing — inspect config:
```powershell
type "%USERPROFILE%\easycopy_config.txt"
```
```bash
cat "$HOME/.easycopy_config.txt"
```
Each line must be `name|source|destination`.

PATH not found — verify:
```bash
which ec        # Linux/macOS
where.exe ec    # Windows
```
On Windows, `setx` needs a new shell session.

Build errors — check compiler and consider `-lstdc++fs` or upgrading.

---

## Security & limitations

- Overwrites files at destination.
- Do not store secrets in shortcuts.
- `|` is the config delimiter — avoid using it in paths.

---

## License & contributions

- License: GPL‑3.0. See LICENSE.
- Contributions welcome: fork, patch, and open a PR. Prefer small, documented changes.

Source in `src/EasyCopy.cpp` is authoritative for CLI behavior.
