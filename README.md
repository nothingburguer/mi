# mi

**mi** is a minimal, Vim-inspired text editor written in **C++** using **ncurses**. It is designed as a lightweight minimal but powerful terminal editor.

---

## Features

- Modal editing (NORMAL, INSERT, COMMAND)
- Line numbers
- Command mode (`:w`, `:q`, `:wq`)
- Status bar with:
  - Current mode
  - Line and column position
  - Open file name
- Colored status bar (mode-aware)
- Cursor navigation with arrow keys
- File loading and saving
- Built-in file manager (FLM)
- Built with **ncurses**

---

## Controls

### NORMAL mode
- `i` — enter INSERT mode
- `ARROW KEYS` — move cursor
- `:` — enter COMMAND mode

### INSERT mode
- Type to insert text
- `ESC` — return to NORMAL mode
- `ARROW KEYS` — move cursor
- `BACKSPACE` — delete character

### COMMAND mode
- `:w` — save file
- `:q` — quit
- `:wq` — save and quit
- `ESC` — cancel command

### Keybinds
- `Ctrl+f` or `:flm` — open the FLM file manager

---

## Dependencies
- GNU GCC/G++
- GNU Make
- ncurses

---

## Building and Running

### STEP 1 - Installing libncurses:


On **Debian** or **Ubuntu**:
```
sudo apt install libncurses-dev
```


On **CentOS/RHEL/Scientific Linux 6.x/7.x+** and **Fedora Linux 21** or older:
```
sudo yum install ncurses-devel
```


On **Fedora Linux 22.x+**:
```
sudo dnf install ncurses-devel
```

<br>

### STEP 2 - Cloning and Installing:


```sh
git clone https://github.com/nothingburguer/mi
cd mi
sudo make install
mi [filename]

# Examples:
#  mi main.cpp
#  mi index.js
#  mi script.py
```

<br>

### ALT 1 - Uninstalling:

(On the repo folder)
```sh
cd mi
sudo make uninstall
```

<br>

### ALT 2 - Local Build

```sh
git clone https://github.com/nothingburguer/mi
cd mi
make

# To clean:
# make clean
```

---

## Screenshots

<img width="1366" height="718" alt="image" src="https://github.com/user-attachments/assets/c437564e-b466-43e0-ae45-16af6a54b802" />

<br>

<img width="1366" height="718" alt="image" src="https://github.com/user-attachments/assets/7fbae362-c978-402f-8abd-0194718999d2" />


---
