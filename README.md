# EasyCopy

EasyCopy is a small, cross-platform command-line utility for quickly copying directories and their contents. It is intended for simple local transfers (game saves, quick backups, simple folder syncs). EasyCopy is easy to build from source and uses a minimal plain-text config to store named shortcuts.

This README describes the current implementation and CLI behavior (see src/EasyCopy.cpp in this repo).

---

## Highlights

- Recursively copy the contents of a source directory into a destination.
- Save repeated copy tasks as named shortcuts (global config in your home directory).
- Run, list, or delete shortcuts from the CLI.
- Validates that the source exists and is a directory before copying.
- Overwrites existing files at the destination.

---

## Quick reference — commands

- Run a saved shortcut:
```bash
ec <shortcutName>
```

- One-off copy (no shortcut saved):
```bash
ec copy "<source>" "<destination>"
```

- Create or update a shortcut (saved to global config):
```bash
ec shortcut <name> copy "<source>" "<destination>"
```

- Delete a saved shortcut:
```bash
ec delete <name>
```

- List saved shortcuts:
```bash
ec list
```

Notes:
- Quote paths that contain spaces.
- The program prints a short usage message when run with no arguments.

---

## Config file & format

- Global config path:
  - Windows: %USERPROFILE%\easycopy_config.txt
  - Linux / macOS: $HOME/.easycopy_config.txt

- Format (one shortcut per line):
```
name|source|destination
```
- The config uses `|` as the field separator so paths may contain spaces but must not contain `|`.
- Use `ec list` to view saved shortcuts, or open the config file in a text editor.

View config (examples):
```powershell
# Windows
type "%USERPROFILE%\easycopy_config.txt"
```
```bash
# Linux / macOS
cat "$HOME/.easycopy_config.txt"
```

---

## Examples

One-off copy (Linux/macOS):
```bash
ec copy "/home/me/saves" "/mnt/backup/saves"
```

One-off copy (Windows PowerShell):
```powershell
ec copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"
```

Create or update a shortcut (saves to the global config):
```bash
ec shortcut mygame copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"
```

Run the saved shortcut:
```bash
ec mygame
```

Delete the shortcut:
```bash
ec delete mygame
```

List shortcuts:
```bash
ec list
```

---

## Installation

Two common workflows: use a prebuilt binary (recommended) or compile locally.

Prebuilt binaries
- Windows:
  1. Download `ec.exe` (64-bit) and place it in a permanent folder, e.g. `C:\Tools\EasyCopy\`.
  2. Add that folder to PATH (PowerShell):
     ```powershell
     setx PATH "$env:PATH;C:\Tools\EasyCopy"
     ```
     Then open a new shell (setx affects new sessions).
  3. Run `ec` to see usage.

  Alternative: use the System → Environment Variables GUI to add the folder to PATH.

- macOS / Linux:
  1. Move the downloaded `ec` into a directory on PATH:
     ```bash
     # system-wide (requires sudo)
     sudo mv ec /usr/local/bin/
     sudo chmod +x /usr/local/bin/ec
     ```
     On Homebrew-managed macOS systems you may prefer `/opt/homebrew/bin`.
  2. Verify:
     ```bash
     which ec
     ec
     ```

Important: prebuilt 32-bit binaries are provided only for Linux. Prebuilt Windows and macOS binaries are 64-bit only. If you need a 32-bit binary on Windows/macOS you must build locally with an appropriate toolchain (see below).

Local compilation (from source)
- Requirements: a C++17 compiler with <filesystem> support. Recommended: GCC 9+, Clang recent versions, or MSVC 2017+ (with filesystem support).
- Typical compile commands:

Linux / macOS (64-bit):
```bash
g++ -std=c++17 src/EasyCopy.cpp -o ec
# or
clang++ -std=c++17 src/EasyCopy.cpp -o ec
```
If using older GCC that lacks full <filesystem> support:
```bash
g++ -std=c++17 src/EasyCopy.cpp -lstdc++fs -o ec
```

Windows (MinGW / MSYS / clang):
```bash
g++ -std=c++17 src/EasyCopy.cpp -o ec.exe
# or
clang++ -std=c++17 src/EasyCopy.cpp -o ec.exe
```

32-bit builds (example for Linux):
```bash
# requires multilib packages such as gcc-multilib
g++ -std=c++17 -m32 src/EasyCopy.cpp -o ec32
```
After building, move the binary to a directory on your PATH (or add its folder to PATH).

---

## Adding EasyCopy to PATH (accurate, per OS)

Windows (PowerShell)
```powershell
# Move ec.exe to a permanent folder such as C:\Tools\EasyCopy
# Add that folder to user PATH
setx PATH "$env:PATH;C:\Tools\EasyCopy"
# Open a new PowerShell or CMD window to use the updated PATH
```
Confirm:
```powershell
where.exe ec
ec
```

macOS (system-wide)
```bash
sudo mv ec /usr/local/bin/
sudo chmod +x /usr/local/bin/ec
which ec
```
macOS (per-user)
```bash
mkdir -p "$HOME/Tools/EasyCopy"
mv ec "$HOME/Tools/EasyCopy/ec"
chmod +x "$HOME/Tools/EasyCopy/ec"
# Add to shell profile (zsh on modern macOS)
echo 'export PATH="$PATH:$HOME/Tools/EasyCopy"' >> ~/.zshrc
source ~/.zshrc
which ec
```

Linux (system-wide)
```bash
sudo mv ec /usr/local/bin/
sudo chmod +x /usr/local/bin/ec
which ec
```
Linux (per-user)
```bash
mkdir -p "$HOME/tools"
mv ec "$HOME/tools/ec"
chmod +x "$HOME/tools/ec"
echo 'export PATH="$PATH:$HOME/tools"' >> ~/.bashrc
source ~/.bashrc
which ec
```

If PATH changes do not appear, open a new terminal session. On Windows, using the GUI to edit Environment Variables may be preferable for long PATH values.

---

## 32-bit systems & compatibility notes

- Prebuilt 32-bit binary: available only for Linux (project releases only provide 32-bit Linux builds).
- Windows and macOS prebuilt releases are 64-bit only. If you need a 32-bit Windows executable, build with a 32-bit MinGW toolchain. macOS no longer supports 32-bit user applications on Catalina (10.15+) and later.
- If compilation fails due to <filesystem> issues on older GCC, try adding `-lstdc++fs` or upgrade to a newer compiler.
- When building 32-bit on Linux, install multilib packages (e.g. `gcc-multilib`).

---

## Troubleshooting (actionable)

"Failed to copy. Check source path."
- Confirm the source exists and is a directory:
```bash
ls -la "/path/to/source"
# or PowerShell
Get-ChildItem "C:\path\to\source"
```
- Verify read permission on source and write permission for destination.

Shortcuts missing or not applied
- Check the global config file location and contents:
```powershell
# Windows
type "%USERPROFILE%\easycopy_config.txt"
```
```bash
# Linux / macOS
cat "$HOME/.easycopy_config.txt"
```
- Each line must be `name|source|destination` (no `|` in paths).

PATH problems
- Confirm `ec` is discoverable:
```bash
which ec        # Linux/macOS
where.exe ec    # Windows
```
- On Windows, `setx` requires a new shell session. Use the Environment Variables GUI if PATH length is an issue.

Build errors
- Check your compiler version:
```bash
g++ --version
clang++ --version
```
- If you see <filesystem> related errors with older GCC, add `-lstdc++fs` or upgrade GCC.

Permission errors
- Prefer running operations as a normal user to destinations you own. Use `sudo` only when truly required.

---

## Security & limitations

- EasyCopy performs local, direct file copies and will overwrite files at the destination.
- Do not store secrets or credentials in shortcuts.
- The config delimiter is `|`; do not include `|` in paths.

---

## Contributing & License

- Contributions are welcome. Fork, make changes, and open a pull request. Keep changes focused and documented.
- License: GPL-3.0. See the LICENSE file in this repository.

---

If anything in this README contradicts your local build or the code in src/EasyCopy.cpp, prefer the source code — it is the definitive behavior.