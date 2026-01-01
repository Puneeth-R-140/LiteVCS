# Contributing to LiteVCS

Thank you for your interest in contributing to LiteVCS! This document provides guidelines and instructions for contributing to this project.

---

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Areas for Contribution](#areas-for-contribution)

---

## 🤝 Code of Conduct

This project is intended as a learning resource. Please be respectful, constructive, and helpful in all interactions.

### Our Standards

- **Be respectful** - Treat everyone with respect and kindness
- **Be constructive** - Provide helpful feedback and suggestions
- **Be patient** - Remember this is a learning project
- **Be collaborative** - Work together to improve the codebase

---

## 🎯 How Can I Contribute?

### Reporting Bugs

If you find a bug, please create an issue with:

- **Clear title** - Describe the bug concisely
- **Steps to reproduce** - Detailed steps to trigger the bug
- **Expected behavior** - What should happen
- **Actual behavior** - What actually happens
- **Environment** - OS, compiler version, dependencies

**Example:**
```
Title: Diff crashes on empty files

Steps to reproduce:
1. Create empty file: touch empty.txt
2. Track file: ./vcs track empty.txt
3. Run diff: ./vcs diff

Expected: No output or "No changes"
Actual: Segmentation fault

Environment: Ubuntu 22.04, g++ 11.3.0
```

### Suggesting Enhancements

Enhancement suggestions are welcome! Please include:

- **Clear description** - What feature you'd like to see
- **Use case** - Why this would be useful
- **Implementation ideas** - How it might work (optional)

### Contributing Code

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

---

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (g++, clang++)
- OpenSSL development libraries
- zlib development libraries
- Git

### Setting Up Development Environment

```bash
# Clone your fork
git clone https://github.com/YOUR-USERNAME/LiteVCS.git
cd LiteVCS

# Add upstream remote
git remote add upstream https://github.com/Puneeth-R-140/LiteVCS.git

# Install dependencies (Ubuntu/Debian)
sudo apt-get install g++ libssl-dev zlib1g-dev

# Compile
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs

# Test
./vcs init
./vcs track readme.md
./vcs save "Test commit"
./vcs history
```

---

## 🔄 Development Workflow

### 1. Create a Feature Branch

```bash
# Update your fork
git checkout main
git pull upstream main

# Create feature branch
git checkout -b feature/your-feature-name
```

### 2. Make Changes

- Write clean, readable code
- Follow existing code style
- Add comments for complex logic
- Update documentation if needed

### 3. Test Your Changes

```bash
# Compile
g++ -std=c++17 src/*.cpp -lssl -lcrypto -lz -o vcs

# Test basic functionality
./vcs init
./vcs track test.txt
./vcs save "Test"
./vcs history
./vcs diff

# Test your specific changes
# ... (add your test cases)
```

### 4. Commit Changes

```bash
git add .
git commit -m "Add feature: brief description"
```

### 5. Push and Create PR

```bash
git push origin feature/your-feature-name
```

Then create a Pull Request on GitHub.

---

## 📝 Coding Standards

### C++ Style Guidelines

#### Naming Conventions

```cpp
// Classes: PascalCase
class Repository { };

// Functions: camelCase
void trackFile(const std::string& path);

// Variables: camelCase
std::string filePath;

// Constants: UPPER_SNAKE_CASE
const int MAX_FILE_SIZE = 1024;

// Private members: camelCase (no prefix)
private:
    std::string vcsDir;
```

#### Code Formatting

```cpp
// Braces on same line for functions
void myFunction() {
    // code
}

// Spaces around operators
int result = a + b;

// Indentation: 4 spaces (no tabs)
if (condition) {
    doSomething();
}

// Include guards in headers
#pragma once
```

#### Comments

```cpp
/**
 * Function-level documentation
 * @param path Path to the file
 * @return Hash of the file
 */
std::string createBlob(const std::string& path);

// Inline comments for complex logic
// Security: Prevent path traversal attacks
if (isPathTraversal(path)) {
    return;
}
```

### Error Handling

```cpp
// Use try-catch for filesystem operations
try {
    std::filesystem::create_directories(path);
} catch (const std::filesystem::filesystem_error& e) {
    std::cout << "Error: " << e.what() << "\n";
}

// Validate input
if (args.size() < 2) {
    std::cout << "Usage: vcs <command>\n";
    return;
}
```

### Security Best Practices

```cpp
// Always validate user input
if (hash.length() > 40) {
    std::cout << "Error: invalid hash\n";
    return;
}

// Check for path traversal
std::filesystem::path canonical = std::filesystem::weakly_canonical(path);
if (!isWithinRepository(canonical)) {
    std::cout << "Error: path traversal detected\n";
    return;
}

// Limit resource usage
const size_t MAX_SIZE = 100 * 1024 * 1024; // 100 MB
if (size > MAX_SIZE) {
    std::cout << "Error: file too large\n";
    return;
}
```

---

## 💬 Commit Guidelines

### Commit Message Format

```
<type>: <subject>

<body>

<footer>
```

### Types

- **feat**: New feature
- **fix**: Bug fix
- **docs**: Documentation changes
- **style**: Code style changes (formatting, no logic change)
- **refactor**: Code refactoring
- **test**: Adding tests
- **chore**: Build process, dependencies, etc.

### Examples

**Good commits:**
```
feat: Add support for branch creation

Implemented basic branching functionality allowing users to create
and switch between branches.

Closes #15
```

```
fix: Prevent crash on empty file diff

Added null check before processing file contents in diff engine.
Now returns "No changes" for empty files.

Fixes #23
```

```
docs: Update README with Windows build instructions

Added detailed MSYS2 setup guide and troubleshooting section
for Windows users.
```

**Bad commits:**
```
fixed stuff
```

```
update
```

```
changes
```

---

## 🔍 Pull Request Process

### Before Submitting

- [ ] Code compiles without errors
- [ ] Code follows project style guidelines
- [ ] All tests pass (manual testing)
- [ ] Documentation updated (if needed)
- [ ] Commit messages are clear and descriptive

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Code refactoring

## Testing
Describe how you tested your changes

## Checklist
- [ ] Code compiles successfully
- [ ] Follows coding standards
- [ ] Documentation updated
- [ ] No breaking changes
```

### Review Process

1. **Automated checks** - Code must compile
2. **Code review** - Maintainer will review your code
3. **Feedback** - Address any requested changes
4. **Approval** - Once approved, PR will be merged

---

## 🎨 Areas for Contribution

### High Priority

- **Unit Tests** - Add Google Test framework and test coverage
- **Performance Optimization** - Improve diff algorithm performance
- **Error Messages** - Make error messages more helpful
- **Cross-Platform Testing** - Test on different OS/compilers

### Medium Priority

- **Branching Support** - Implement branch creation and switching
- **Merge Functionality** - Add basic merge capabilities
- **Ignore Files** - Support for `.vcsignore` file
- **Better Diff Output** - Colorized diff output

### Low Priority

- **Remote Repositories** - Network sync capabilities
- **GUI Tool** - Simple graphical interface
- **Language-Aware Diff** - Better semantic diff for different languages
- **Commit Analytics** - Statistics and insights

### Documentation

- **Tutorial Videos** - Screen recordings of usage
- **Blog Posts** - Write-ups about internals
- **Code Examples** - More usage examples
- **API Documentation** - Doxygen-style documentation

---

## 🐛 Bug Bounty

This is a learning project, so there's no monetary reward. However:

- **Recognition** - Contributors listed in README
- **Learning** - Great experience with systems programming
- **Portfolio** - Showcase your contributions

---

## 📚 Resources

### Learning Materials

- [Git Internals](https://git-scm.com/book/en/v2/Git-Internals-Plumbing-and-Porcelain)
- [C++ Best Practices](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Filesystem Library](https://en.cppreference.com/w/cpp/filesystem)

### Project Resources

- **Main Repository**: https://github.com/Puneeth-R-140/LiteVCS
- **Issue Tracker**: https://github.com/Puneeth-R-140/LiteVCS/issues
- **Discussions**: https://github.com/Puneeth-R-140/LiteVCS/discussions

---

## ❓ Questions?

If you have questions about contributing:

1. Check existing issues and discussions
2. Create a new discussion thread
3. Reach out to maintainers

---

## 🙏 Thank You!

Every contribution, no matter how small, helps make LiteVCS better. Whether you're fixing a typo, reporting a bug, or adding a major feature, your effort is appreciated!

**Happy Contributing!** 🚀

---

**Maintainer**: Puneeth R (GHOST) / Puneeth-R-140  
**License**: MIT  
**Project**: LiteVCS - A Lightweight Version Control System
