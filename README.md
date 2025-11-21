# EasyCopy

EasyCopy is a small command-line tool I created specifically for myself to copy game save files quickly.  
It may not be suitable for everyone, but it is simple, easy to compile, and easy to modify.

A strong recommendation: **add the `ec` executable to your system PATH** so you can call it from anywhere.  
A full guide is included below.

---

## Features

- Copy files or entire directories  
- Create named shortcuts for frequently used copy operations  
- Execute saved shortcuts by name  
- Delete shortcuts  
- Validates source paths before copying  
- Stores shortcuts in a simple config file (`easycopy_config.cpp`)  
- Fully cross-platform (Windows, macOS, Linux)  

---

# Usage

Below are all supported operations.

---

## 1. Execute a Shortcut

ec <shortcutName>


Example:

ec bg3


Runs the previously saved command associated with `bg3`.

---

## 2. Create a Shortcut

ec shortcut <name> copy "<source>" "<destination>"


Example:

ec shortcut bg3 copy "C:\Games\BG3\Saves" "D:\Backup\BG3"


This adds a line to `easycopy_config.cpp` storing your copy command.

---

## 3. Delete a Shortcut

ec delete <name>


Example:

ec delete bg3


---

## 4. Copy Without Using Shortcuts

ec copy "<source>" "<destination>"


Example:

ec copy "/home/me/saves" "/mnt/backup/saves"


---

# Adding EasyCopy to Your PATH (Recommended)

Adding `ec` to your PATH lets you run it from any directory without typing the full path to the executable.

Below is a complete beginner-friendly guide for all platforms.

---

# Windows PATH 

### Step 1 — Choose a permanent folder  
Create a directory such as:

C:\Tools\EasyCopy


Place your `ec.exe` inside it.

### Step 2 — Add it to PATH  
Open PowerShell and run:

setx PATH "$env:PATH;C:\Tools\EasyCopy"


### Step 3 — Restart PowerShell  
Now you should be able to run:

ec


from anywhere.

---

# macOS PATH 

### Step 1 — Move the executable  
Run:

sudo mv ec /usr/local/bin/
sudo chmod +x /usr/local/bin/ec


This places EasyCopy in a directory that’s already on PATH.

### Alternative Method  
If you prefer a custom directory:

1. Create a folder, e.g.:

mkdir -p ~/Tools/EasyCopy


2. Move the binary:

mv ec ~/Tools/EasyCopy/


3. Edit your `~/.zshrc`:

export PATH="$PATH:$HOME/Tools/EasyCopy"


4. Reload:

source ~/.zshrc


You can now run:

ec


globally.

---

# Linux PATH 

### Method 1 — The standard way  
Move the binary:

sudo mv ec /usr/local/bin/
sudo chmod +x /usr/local/bin/ec


This works for most distros.

### Method 2 — Custom directory  
1. Create a tool directory:

mkdir -p ~/tools


2. Move `ec`:

mv ec ~/tools/


3. Add to PATH by editing `~/.bashrc`:

export PATH="$PATH:$HOME/tools"


4. Reload:

source ~/.bashrc


Now `ec` runs from anywhere.

---

# Compiling EasyCopy

Requires a C++17 compiler with filesystem support.

---

## Windows (Clang or MinGW)

clang++ -std=c++17 EasyCopy.cpp -o ec.exe


or

g++ -std=c++17 EasyCopy.cpp -o ec.exe


---

## macOS

clang++ -std=c++17 EasyCopy.cpp -o ec


---

## Linux

g++ -std=c++17 EasyCopy.cpp -o ec


If needed:

sudo apt install build-essential


---

# Notes

- Shortcuts are read from and written to `easycopy_config.cpp`  
- Manual editing is allowed but formatting must match what the program generates  
- Quote any path containing spaces  
- Copying is recursive and overwrites existing files