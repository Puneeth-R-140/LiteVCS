# LiteVCS Quick Reference Guide

## 🔨 Compilation

### Quick Compile (All Platforms)
```bash
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs
```

**Windows:**
```powershell
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs.exe
```

### Alternative Build Methods

**CMake:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**Makefile:**
```bash
make
```

---

## 📝 Basic Commands

| Command | Description | Example |
|---------|-------------|---------|
| `vcs init` | Initialize repository | `./vcs init` |
| `vcs track <file>` | Track a file | `./vcs track readme.md` |
| `vcs save "message"` | Commit changes | `./vcs save "Initial commit"` |
| `vcs history` | Show commit history | `./vcs history` |
| `vcs go <hash>` | Checkout commit | `./vcs go d2db873e` |
| `vcs diff` | Show line-by-line changes | `./vcs diff` |
| `vcs diff --smart` | Show function-level changes | `./vcs diff --smart` |

---

## 🎯 Diff Options

| Option | Description | Use Case |
|--------|-------------|----------|
| `--smart` | Semantic/function-level diff | Code reviews |
| `--ignore-empty` | Ignore empty lines | Focus on content |
| `--ignore-whitespace` | Ignore whitespace changes | Formatting changes |

**Combine options:**
```bash
./vcs diff --smart --ignore-empty --ignore-whitespace
```

---

## 🚀 Common Workflows

### First Time Setup
```bash
# Compile
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs

# Initialize
./vcs init

# Track files
./vcs track src/main.cpp
./vcs track readme.md

# First commit
./vcs save "Initial commit"
```

### Daily Workflow
```bash
# Make changes to files...

# Check what changed
./vcs diff

# Commit changes
./vcs save "Added new feature"

# View history
./vcs history
```

### Code Review Workflow
```bash
# See which functions changed
./vcs diff --smart

# See detailed line changes
./vcs diff

# Ignore formatting changes
./vcs diff --ignore-whitespace
```

### Time Travel
```bash
# View history
./vcs history

# Go back to previous commit
./vcs go d2db873e

# Return to latest
./vcs go <latest-hash>
```

---

## 🐛 Troubleshooting

### Windows - g++ not found
```bash
# Install MSYS2 from https://www.msys2.org/
# Open MSYS2 MinGW 64-bit terminal
pacman -S mingw-w64-x86_64-gcc
```

### Windows - Cannot find -lssl
```bash
# Install OpenSSL and zlib
pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-zlib
```

### Linux - Missing libraries
```bash
# Ubuntu/Debian
sudo apt-get install g++ libssl-dev zlib1g-dev

# Fedora/RHEL
sudo dnf install gcc-c++ openssl-devel zlib-devel
```

### macOS - Missing libraries
```bash
brew install openssl zlib
```

### "Error: not a LiteVCS repository"
```bash
# Run init first
./vcs init
```

### "Error: path traversal detected"
```bash
# File must be within repository directory
# Don't use: ./vcs track ../../../etc/passwd
# Use: ./vcs track myfile.txt
```

---

## 📊 Understanding Diff Types

### Regular Diff
Shows **exact line changes**:
```
diff -- test.txt
+ New line added
- Old line removed
```

**Best for:** Any file type, detailed changes

### Smart Diff
Shows **function-level changes**:
```
smart-diff -- main.cpp

Modified function: main()
Modified function: calculateTotal()
```

**Best for:** Code files, high-level overview

---

## 🔧 Advanced Usage

### Track Multiple Files
```bash
./vcs track src/main.cpp
./vcs track src/utils.cpp
./vcs track readme.md
./vcs save "Added multiple files"
```

### View Specific Commit
```bash
# Full hash or short hash works
./vcs go d2db873e
./vcs go d2db873e8a1f2c3d4e5f6a7b8c9d0e1f2a3b4c5d
```

### Compare with Previous Commit
```bash
# Make changes...
./vcs diff                    # Line-by-line
./vcs diff --smart            # Function-level
```

---

## 📁 Repository Structure

```
.vcs/
├── objects/
│   ├── blobs/      # Compressed file contents
│   ├── trees/      # Directory snapshots
│   └── commits/    # Commit metadata
├── index           # Tracked files list
├── HEAD            # Current commit pointer
└── config          # Repository settings
```

---

## 💡 Tips & Best Practices

1. **Commit often** - Small, focused commits are better
2. **Use descriptive messages** - `./vcs save "Fixed login bug"` not `./vcs save "fix"`
3. **Check diff before commit** - Always run `./vcs diff` first
4. **Use smart diff for code** - Great for understanding impact of changes
5. **Short hashes work** - `./vcs go d2db` instead of full hash

---

## 🔗 Quick Links

- **Full Documentation:** [readme.md](readme.md)
- **Source Code:** [src/](src/)
- **License:** [LICENSE](LICENSE)
- **Issues:** Report on GitHub

---

## 📞 Getting Help

```bash
# Run without arguments for help
./vcs

# Output:
# Usage: vcs <command>
# 
# Available commands:
#   init                     - Initialize a new repository
#   track <file>             - Track a file
#   save "message"           - Commit changes
#   history                  - Show commit history
#   go <commit_hash>         - Checkout a commit
#   diff [options]           - Show changes
```

---

**Happy Version Controlling!** 🚀
