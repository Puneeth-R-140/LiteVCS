
# 📘 LiteVCS — A Lightweight Version Control System (C++)

LiteVCS is a Git-inspired, lightweight version control system built from scratch in **C++** to understand and implement the **core internals of modern VCS tools** — including content-addressed storage, immutable commits, diff engines, and history traversal.

> ⚠️ LiteVCS is **not a Git replacement**.
> It is a **learning-oriented, systems-level implementation** focused on correctness, clarity, and engineering trade-offs.

---

## ⚡ Quick Start

```bash
# Compile
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs

# Use
./vcs init
./vcs track myfile.txt
./vcs save "Initial commit"
./vcs history
```

**Windows users:** Replace `vcs` with `vcs.exe` and use PowerShell or MSYS2 terminal.

📖 **For detailed commands and workflows, see [QUICKSTART.md](QUICKSTART.md)**

---

## 🚀 Key Features

### ✅ Core Version Control

* Repository initialization
* File staging (index)
* Immutable commits using **SHA-1**
* Content deduplication via hashing
* Commit history traversal
* Time-travel (checkout to previous commits)

### 🔍 Diff Engine

* **LCS-based line diff** (handles insertions & deletions correctly)
* Smart / semantic diff (function-level change detection)
* User-controlled noise suppression:

  * `--ignore-empty`
  * `--ignore-whitespace`

### 🛡️ Hardening & Safety

* Prevents empty commits
* Detects deleted files explicitly
* Safe, bounded decompression
* Clear, defensive error handling
* Cross-platform path normalization

---

## 🧠 Design Philosophy

LiteVCS follows the same core principles as Git:

* **Content-addressed storage**
  Objects are identified by cryptographic hashes of their content.

* **Immutability**
  Commits cannot be modified — changes create new commits.

* **Separation of concerns**

  * Blobs → file content
  * Trees → directory snapshots
  * Commits → history & metadata

* **User control over defaults**
  Whitespace and formatting are treated as real changes unless explicitly ignored.

---

## 📁 Repository Structure

```text
.vcs/
├── objects/
│   ├── blobs/      # file contents (compressed)
│   ├── trees/      # snapshots
│   └── commits/    # commit metadata
├── index           # staging area
├── HEAD            # current commit pointer
└── config          # repository metadata
```

---

## 🛠️ Build Instructions

### Requirements

* C++17 or newer
* OpenSSL (for SHA-1)
* zlib (for compression)

### Compile

#### Option 1: Direct Compilation (Recommended for Quick Start)

**Linux/macOS:**
```bash
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs
```

**Windows (MinGW/MSYS2):**
```bash
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs.exe
```

**Windows (PowerShell):**
```powershell
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs.exe
```

#### Option 2: Using CMake (Cross-Platform)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The executable will be in the `build/` directory.

#### Option 3: Using Makefile (Unix-like systems)

```bash
make
```

This creates the `vcs` executable in the current directory.

### Quick Test

After compilation, verify it works:

```bash
./vcs init
./vcs track readme.md
./vcs save "Test commit"
./vcs history
```

---

## 📌 Usage

### Initialize Repository

```bash
vcs init
```

### Track Files

```bash
vcs track src/main.cpp
```

### Commit Changes

```bash
vcs save "Initial commit"
```

### View History

```bash
vcs history
```

### Checkout a Commit

```bash
vcs go <commit_hash>
```

(short hashes are supported, Git-style)

---

## 🔎 Diff Examples

### Line Diff

```bash
vcs diff
```

```diff
diff -- src/main.cpp
+ // added logging
```

### Ignore Empty Lines

```bash
vcs diff --ignore-empty
```

### Ignore Whitespace

```bash
vcs diff --ignore-whitespace
```

### Smart / Semantic Diff

```bash
vcs diff --smart
```

```text
smart-diff -- src/main.cpp

Modified function: main()
```

---

## 🧪 What This Project Demonstrates

This project intentionally focuses on **depth over breadth** and demonstrates:

* Systems programming in C++
* File-system–backed databases
* Cryptographic hashing & compression
* Dynamic programming (LCS diff)
* Defensive engineering & invariants
* Tool usability & developer experience

---

## 🧩 Known Limitations

* No branching or merging
* No networking / remote repositories
* Single-user, local-only design

These are **deliberate design choices** to keep the focus on core internals.

---

## 📈 Future Work (Optional)

* Branching & merge strategies
* Commit analytics / repo insights
* Improved language-aware diffing
* Performance optimizations for large files

---

## 🎯 Why LiteVCS?

> “I built LiteVCS to deeply understand how version control systems work internally — not just how to use them.”

This project mirrors **real engineering problems**:

* silent corruption
* invariant violations
* diff correctness
* usability trade-offs

---

## 🧑‍💻 Author

Built by **GHOST** aka **Puneeth R**
Focused on systems engineering, tooling, and low-level design.

---

## 🔧 Troubleshooting

### Build Issues

**OpenSSL not found:**
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# macOS
brew install openssl
export OPENSSL_ROOT_DIR=/usr/local/opt/openssl
```

**zlib not found:**
```bash
# Ubuntu/Debian
sudo apt-get install zlib1g-dev

# macOS (usually pre-installed)
brew install zlib
```

**Windows (MinGW/MSYS2):**
```bash
# Install dependencies via MSYS2
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl mingw-w64-x86_64-zlib

# Add to PATH (if not already)
# C:\msys64\mingw64\bin
```

**Compilation errors on Windows:**

If you get `g++: command not found`:
```bash
# Install MinGW-w64 via MSYS2
# Download from: https://www.msys2.org/
# Then install g++:
pacman -S mingw-w64-x86_64-gcc
```

If you get linker errors (`cannot find -lssl`):
```bash
# Make sure OpenSSL and zlib are installed
pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-zlib

# Or specify library paths explicitly:
g++ -std=c++17 src/*.cpp -LC:/msys64/mingw64/lib -lssl -lcrypto -lz -o vcs.exe
```

**Quick Windows Setup (MSYS2):**
```bash
# 1. Install MSYS2 from https://www.msys2.org/
# 2. Open MSYS2 MinGW 64-bit terminal
# 3. Install tools:
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl mingw-w64-x86_64-zlib

# 4. Navigate to your project and compile:
cd /d/LiteVCS
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs.exe
```

### Runtime Issues

**"Error: not a LiteVCS repository"**
- Run `vcs init` in your project directory first

**"Error: path traversal detected"**
- File must be within the repository directory
- Avoid using `../` in file paths

**Decompression errors:**
- Repository may be corrupted
- Try re-initializing with `vcs init`

---

## 🔒 Security Considerations

### Educational Purpose

LiteVCS is designed for **learning** and **demonstration** purposes:

* **SHA-1 Usage**: Uses SHA-1 for Git compatibility. Production systems should use SHA-256 or stronger.
* **Path Traversal Protection**: Validates file paths to prevent directory traversal attacks.
* **Decompression Limits**: Prevents decompression bombs with 100MB size limit.
* **Input Validation**: Validates commit hashes and user inputs.

### Not Recommended For

* Production version control
* Sensitive or critical data
* Large-scale projects (>100MB files)

---

## 🤝 Contributing

This is a learning project, but suggestions are welcome!

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

**Areas for contribution:**
* Additional diff algorithms
* Performance optimizations
* Better error messages
* Cross-platform testing

---

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

Copyright (c) 2026 Puneeth R (GHOST)

---

