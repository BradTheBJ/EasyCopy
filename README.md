# EasyCopy

EasyCopy is a small, cross-platform command-line utility for quickly copying directories and their contents. It's intended for simple local transfers (game saves, quick backups). Shortcuts are stored in a minimal plain-text config file.

See implementation: `src/EasyCopy.cpp`.

---

## Highlights

- Recursively copy a source directory into a destination (creates destination dirs, overwrites files).
- Save, run, list, and delete named shortcuts.
- Global config stored in your home directory.
- Validates source exists and is a directory before copying.
- Confirmation prompt appears when source folder is not empty (for direct copies and shortcut creation).
- Optional verbose mode shows each file and directory operation.

---

## Quick commands

Run a saved shortcut:
```bash
ec <shortcutName> [verbose]
```

One-off copy:
```bash
ec copy "<source>" "<destination>" [verbose]
```

Create/update shortcut:
```bash
ec shortcut <name> copy "<source>" "<destination>" [verbose]
```

Delete shortcut:
```bash
ec delete <name>
```

List shortcuts:
```bash
ec list
```

Running with no arguments displays usage information.

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
# With verbose output:
ec copy "/home/me/saves" "/mnt/backup/saves" verbose
```

Windows PowerShell one-off:
```powershell
ec copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"
```

Create, run, delete:
```bash
# Create shortcut (saves absolute paths)
ec shortcut mygame copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"

# Create shortcut and immediately copy with verbose output
ec shortcut mygame copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves" verbose

# Run saved shortcut
ec mygame

# Run with verbose output
ec mygame verbose

# Delete shortcut
ec delete mygame

# List all shortcuts
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

macOS
1. Download the 64‑bit macOS prebuilt binary.
2. Rename the downloaded file to `ec` and move to a PATH directory:
```bash
mv downloaded-file ec
chmod +x ec
sudo mv ec /usr/local/bin/
```
3. Verify:
```bash
which ec
ec
```

Linux
1. Rename the downloaded file to `ec` and move to a PATH directory:
```bash
mv downloaded-file ec
chmod +x ec
sudo mv ec /usr/local/bin/
```
2. Verify:
```bash
which ec
ec
```

Note: Prebuilt binaries are available for Windows (64‑bit only), macOS (64‑bit only), and Linux (32‑bit and 64‑bit). If you need a 32‑bit binary on Windows or macOS, build locally.

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

## Behavior details

**Confirmation prompts:**
- Direct copies (`ec copy`) prompt for confirmation if the source folder is not empty
- Saved shortcuts prompt for confirmation if the source folder is not empty when run
- Empty source folders do not prompt for confirmation
- User must enter `y` or `Y` to proceed; any other input cancels the operation

**Verbose mode:**
- Add `verbose` as the last argument to any command
- Shows `[DIR ] Created: <path>` for each directory
- Shows `[FILE] Copied: <source> -> <destination>` for each file
- When creating a shortcut with `verbose`, it saves the shortcut AND immediately performs the copy with verbose output

**Shortcut storage:**
- Shortcuts store absolute paths (converted via `fs::absolute()`)
- Paths are stored exactly as provided after conversion to absolute form

**File operations:**
- Overwrites existing files at destination (`fs::copy_options::overwrite_existing`)
- Creates destination directories as needed
- Uses recursive iteration to copy all nested files and folders

---

## Troubleshooting

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
