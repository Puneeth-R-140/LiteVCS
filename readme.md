
# LiteVCS - A Lightweight Version Control System

Ever wondered how Git actually works under the hood? I built LiteVCS from scratch in C++ to understand the core concepts of version control systems - content-addressed storage, immutable commits, diff algorithms, and history management.

**Note:** This isn't meant to replace Git. It's a learning project focused on understanding how VCS tools work internally.

---

## Quick Start

```bash
# Compile
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs

# Use it
./vcs init
./vcs track myfile.txt
./vcs save "Initial commit"
./vcs history
```

**Windows users:** Use `vcs.exe` instead of `vcs` and run from PowerShell or MSYS2.

For more commands and examples, check out [QUICKSTART.md](QUICKSTART.md).

---

## What It Does

### Core Features

- Initialize repositories
- Track and stage files
- Create immutable commits (using SHA-1)
- View commit history
- Time-travel to previous commits
- Smart diff engine with LCS algorithm

### Diff Capabilities

- **Line-by-line diff** - See exact changes
- **Smart/semantic diff** - Function-level change detection
- **Flexible filtering** - Ignore empty lines or whitespace

---

## How It Works

LiteVCS follows Git's design principles:

**Content-Addressed Storage**  
Files are stored by their hash, not their name. Same content = same hash = stored once.

**Immutability**  
Commits can't be modified. Changes create new commits.

**Simple Structure**  
- Blobs store file contents
- Trees store directory snapshots
- Commits store metadata and history

**User Control**  
Whitespace and formatting are treated as real changes unless you explicitly ignore them.

---

## Repository Structure

```
.vcs/
├── objects/
│   ├── blobs/      # compressed file contents
│   ├── trees/      # directory snapshots
│   └── commits/    # commit metadata
├── index           # tracked files
├── HEAD            # current commit
└── config          # repo settings
```

---

## Building

### Requirements

- C++17 compiler (g++, clang++)
- OpenSSL (for SHA-1 hashing)
- zlib (for compression)

### Option 1: Direct Compilation (Easiest)

**Linux/macOS:**
```bash
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs
```

**Windows (MSYS2/MinGW):**
```bash
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs.exe
```

### Option 2: CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Option 3: Makefile

```bash
make
```

### Quick Test

```bash
./vcs init
./vcs track readme.md
./vcs save "Test commit"
./vcs history
```

---

## Usage

### Initialize a Repository

```bash
vcs init
```

### Track Files

```bash
vcs track src/main.cpp
vcs track readme.md
```

### Commit Changes

```bash
vcs save "Added new feature"
```

### View History

```bash
vcs history
```

Output:
```
commit d2db873e
Date: 1767018788
Message: initial commit 2

commit 776d0bdb
Date: 1767017381
Message: initial commit
```

### Go Back in Time

```bash
vcs go d2db873e    # short hash works
```

---

## Diff Examples

### Basic Diff

```bash
vcs diff
```

Output:
```
diff -- test.txt
+ New line added
- Old line removed
```

### Ignore Empty Lines

```bash
vcs diff --ignore-empty
```

### Ignore Whitespace

```bash
vcs diff --ignore-whitespace
```

### Smart Diff (Function-Level)

```bash
vcs diff --smart
```

Output:
```
smart-diff -- main.cpp

Modified function: main()
Modified function: calculateTotal()
```

**When to use what:**
- Use regular diff when you want to see exact line changes
- Use smart diff when you want a high-level overview of which functions changed

---

## What I Learned

Building this taught me:

- How content-addressed storage works
- Cryptographic hashing and compression
- Dynamic programming (LCS algorithm for diffs)
- File system operations in C++
- Defensive programming and error handling
- The trade-offs in tool design

---

## Known Limitations

These are intentional to keep the project focused:

- No branching or merging
- No remote repositories
- Single-user, local-only
- No staging area modifications

---

## Troubleshooting

### Build Issues

**Can't find OpenSSL:**
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev

# macOS
brew install openssl
```

**Can't find zlib:**
```bash
# Ubuntu/Debian
sudo apt-get install zlib1g-dev

# macOS
brew install zlib
```

**Windows - g++ not found:**
```bash
# Install MSYS2 from https://www.msys2.org/
# Then in MSYS2 terminal:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl mingw-w64-x86_64-zlib
```

**Windows - Linker errors:**
```bash
# Make sure libraries are installed
pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-zlib

# Or specify paths explicitly:
g++ -std=c++17 src/*.cpp -LC:/msys64/mingw64/lib -lssl -lcrypto -lz -o vcs.exe
```

### Runtime Issues

**"Error: not a LiteVCS repository"**  
Run `vcs init` first.

**"Error: path traversal detected"**  
File must be inside the repository. Don't use `../` in paths.

**Decompression errors**  
Repository might be corrupted. Try `vcs init` in a fresh directory.

---

## Security Notes

This is an educational project. Some things to know:

- **SHA-1 is used for Git compatibility** - Production systems should use SHA-256
- **Path traversal protection** - Validates file paths to prevent attacks
- **Decompression limits** - 100MB cap to prevent memory bombs
- **Input validation** - Checks commit hashes and user input

**Don't use this for:**
- Production version control
- Sensitive data
- Large files (>100MB)

---

## Contributing

Want to help improve LiteVCS? Awesome!

Check out [CONTRIBUTING.md](CONTRIBUTING.md) for:
- Development setup
- Coding standards
- How to submit changes

**Quick contribution guide:**
1. Fork the repo
2. Create a branch: `git checkout -b feature/cool-feature`
3. Make your changes
4. Test it works
5. Submit a PR

**Ideas for contributions:**
- Add unit tests
- Optimize the diff algorithm
- Improve error messages
- Test on different platforms
- Add branching support
- Better documentation

---

## Future Ideas

Things I might add (or you could contribute):

- Branch and merge support
- Commit analytics
- Language-aware diffs
- Performance optimizations for large repos

---

## License

MIT License - see [LICENSE](LICENSE) for details.

Feel free to use this code for learning, teaching, or building your own tools.

---

## Author

Built by **Puneeth R** (GHOST)

I'm interested in systems programming, low-level design, and understanding how tools work internally.

---

## Why I Built This

> "I wanted to deeply understand how version control works - not just how to use it, but how it actually works under the hood."

This project helped me understand real engineering problems like silent corruption, invariant violations, diff correctness, and usability trade-offs.

If you're learning systems programming or want to understand Git better, feel free to explore the code and ask questions!

---

**Star this repo if you found it useful!** ⭐
