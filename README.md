
# EasyCopy

EasyCopy is a small, cross‑platform command‑line utility for quickly copying directories and their contents. It is intended for simple local transfers (game saves, quick backups, etc.), is easy to build from source, and stores simple named shortcuts for repeated copy tasks.

> **Note:** This tool was originally developed for personal use. While it is functional across Windows, Linux, and macOS, it may not cover every use case or meet all user expectations. Use at your own discretion.


---

## Highlights

- Recursive copy of a source directory into a destination (creates destination directories, overwrites existing files).
- Save repeated copy tasks as named shortcuts.
- Run, list, or delete shortcuts.
- Shortcuts stored in a single plain‑text config file in your home directory.

---

## Config & Format

- Global config file:
  - Windows: %USERPROFILE%\easycopy_config.txt
  - Linux / macOS: $HOME/.easycopy_config.txt
- Each line in the config uses the pipe (|) delimiter:
  name|source|destination
- Paths may contain spaces but must not contain the pipe character (|).
- Use `ec list` to print saved shortcuts or open the config directly to inspect/edit.

View config:
```powershell
# Windows
type "%USERPROFILE%\easycopy_config.txt"
```
```bash
# Linux / macOS
cat "$HOME/.easycopy_config.txt"
```

---

## Quick Usage

Run a saved shortcut:
```bash
ec <shortcutName>
```

Create or update a shortcut (saved to global config):
```bash
ec shortcut <name> copy "<source>" "<destination>"
```

Delete a shortcut:
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

Notes:
- Always quote paths that contain spaces.
- The program validates the source path exists and is a directory before copying.

---

## Examples (consistent)

```bash
# One-off (Linux/macOS)
ec copy "/home/me/saves" "/mnt/backup/saves"

# One-off (Windows PowerShell)
ec copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"

# Save a shortcut
ec shortcut mygame copy "C:\Games\BG3\Saves" "D:\Backup\Game Saves"

# Run saved shortcut
ec mygame

# Delete saved shortcut
ec delete mygame

# List shortcuts
ec list
```

---

## Installation

Two common workflows: using a prebuilt binary, or compiling locally.

Prebuilt binary (recommended)
- Windows:
  1. Download ec.exe and place it in a permanent folder, e.g. `C:\Tools\EasyCopy\`.
  2. Add that folder to PATH (PowerShell):
     ```powershell
     setx PATH "$env:PATH;C:\Tools\EasyCopy"
     ```
     Restart your shell (or log out/in) for the change to take effect.
  3. Run: `ec` or `ec copy ...`

  Alternatively use the System -> Environment Variables GUI to add the folder to PATH.

- macOS / Linux:
  1. Move the downloaded binary to a directory already on PATH:
     ```bash
     # system-wide (requires sudo)
     sudo mv ec /usr/local/bin/
     sudo chmod +x /usr/local/bin/ec
     ```
     On Apple Silicon Homebrew systems, `/opt/homebrew/bin` may be appropriate.
  2. Verify with `which ec` or `ec --help`.

Local compilation
- Requirements: C++17 compiler with <filesystem> support (GCC 9+, Clang recent versions, MSVC 2017+).
- Common compile commands:
  - Linux / macOS:
    ```bash
    g++ -std=c++17 src/EasyCopy.cpp -o ec
    # or
    clang++ -std=c++17 src/EasyCopy.cpp -o ec
    ```
    On older GCC (<9) you may need `-lstdc++fs`:
    ```bash
    g++ -std=c++17 src/EasyCopy.cpp -lstdc++fs -o ec
    ```
  - Windows (MSYS/MinGW or clang):
    ```bash
    g++ -std=c++17 src/EasyCopy.cpp -o ec.exe
    # or
    clang++ -std=c++17 src/EasyCopy.cpp -o ec.exe
    ```
  - 32-bit builds (if required):
    ```bash
    g++ -std=c++17 -m32 src/EasyCopy.cpp -o ec    # may require multilib packages on Linux (e.g. gcc-multilib)
    ```

After building, move the binary to a PATH location or add its folder to your PATH as shown above.

---

## PATH — common scenarios

- Windows PowerShell:
  ```powershell
  setx PATH "$env:PATH;C:\Tools\EasyCopy"
  # then restart shell
  ```

- macOS / Linux (per-user custom folder):
  ```bash
  mkdir -p "$HOME/tools"
  mv ec "$HOME/tools/"
  chmod +x "$HOME/tools/ec"
  echo 'export PATH="$PATH:$HOME/tools"' >> ~/.bashrc   # or ~/.zshrc
  source ~/.bashrc
  ```

- System-wide install uses `/usr/local/bin` (requires sudo).

Note: `setx` updates the user environment for new shells; open a new terminal window after using it.

---

## 32‑bit systems & compatibility notes

- Many modern macOS and Linux distributions are 64‑bit only; macOS dropped 32‑bit app support starting with Catalina (10.15).
- To build a 32‑bit binary on Linux, install multilib toolchains (e.g. `gcc-multilib`) and compile with `-m32`.
- On Windows, use a 32‑bit toolchain (MinGW 32‑bit) if you need a 32‑bit executable.
- If compilation fails due to <filesystem> issues, try a newer compiler or add `-lstdc++fs` when using older GCC versions.

---

## Troubleshooting (actionable)

- "Failed to copy. Check source path."
  - Ensure the source exists and is a directory:
    ```bash
    ls -la "/path/to/source"
    ```
  - Confirm permissions (read for source, write for destination).

- Shortcuts missing or not applied:
  - Confirm config file location and contents:
    ```bash
    # Windows
    type "%USERPROFILE%\easycopy_config.txt"

    # Linux/macOS
    cat "$HOME/.easycopy_config.txt"
    ```
  - Each line must be: name|source|destination

- PATH issues:
  - Confirm `ec` is on PATH:
    ```bash
    which ec    # Linux/macOS
    where.exe ec  # Windows
    ```
  - On Windows, after `setx` you must start a new shell.

- Build errors:
  - Check your compiler version:
    ```bash
    g++ --version
    clang++ --version
    ```
  - For older compilers, add `-lstdc++fs` or upgrade the compiler.

- Permission errors:
  - On Linux/macOS, use `sudo` only if destination requires root ownership; prefer a location you own.

---

## Security & Limitations

- EasyCopy performs direct file copies and will overwrite files at the destination.
- Avoid storing sensitive credentials in shortcuts or using remote/automated transfers — EasyCopy is for local filesystem operations.
- The config delimiter is `|`; do not include `|` in paths.

---

## Contributing & License

- EasyCopy is licensed under GPL‑3.0. See the LICENSE file.
- Contributions welcome: fork, modify, and send pull requests. Keep changes minimal and well‑documented.

---
