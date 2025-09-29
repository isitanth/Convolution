# Build Script Documentation

This document describes how to use the `build.sh` script for the Convolution Explorer project.

## Overview

The `build.sh` script is a comprehensive build and cleanup tool designed specifically for macOS (and other Unix-like systems). It provides an easy way to:

- ✅ Build the project in various configurations
- 🚀 Run the application 
- 🧹 Clean up all build artifacts
- 📊 Check build status
- 🎯 Interactive mode for easy navigation

## Quick Start

```bash
# Make the script executable (first time only)
chmod +x build.sh

# Interactive mode - shows a menu
./build.sh

# Build the project
./build.sh build

# Build and run
./build.sh run

# Clean everything
./build.sh clean

# Check status
./build.sh status
```

## Usage

```
./build.sh [COMMAND] [OPTIONS]
```

### Commands

| Command | Description | Examples |
|---------|-------------|----------|
| `build [TYPE]` | Build the project | `./build.sh build`<br>`./build.sh build debug`<br>`./build.sh build release` |
| `run` | Build (if needed) and run the application | `./build.sh run` |
| `clean` | Remove all build artifacts | `./build.sh clean` |
| `status` | Show current build status | `./build.sh status` |
| `help` | Show help message | `./build.sh help` |
| (no command) | Interactive mode | `./build.sh` |

### Build Types

| Type | Description | Use Case |
|------|-------------|----------|
| `all` | Build everything (default) | General development |
| `debug` | Build with debug symbols | Debugging, development |
| `release` | Optimized build | Production, distribution |
| `examples` | Build example programs only | Testing specific features |
| `main` | Build main application only | Quick testing |

## Interactive Mode

When you run `./build.sh` without arguments, you get an interactive menu:

```
╔══════════════════════════════════════════════════════════╗
║                    Convolution Explorer                  ║
║                   Build & Cleanup Script                ║
║                      Version 1.0.0                      ║
╚══════════════════════════════════════════════════════════╝

ℹ Current build status:
✓ Project is clean (no build artifacts found)

What would you like to do?
1) Build project (all)
2) Build debug version
3) Build release version
4) Build examples only
5) Run application
6) Clean project
7) Show build status
8) Show help
9) Exit

Enter your choice (1-9):
```

## Features

### 🔍 Smart Status Checking
- Shows which build artifacts exist
- Counts files in build directories
- Identifies temporary files
- Provides clear clean/dirty status

### 🛠 Multiple Build Configurations
- **All**: Complete build with main app and examples
- **Debug**: Build with debugging symbols (`-g -DDEBUG -O0`)
- **Release**: Optimized build (`-O3 -DNDEBUG -s`)
- **Examples**: Just the example programs
- **Main**: Only the main application

### 🧹 Comprehensive Cleanup
- Uses `make clean` first (safest approach)
- Removes build directories: `bin/`, `obj/`, `lib/`
- Cleans temporary files: `*.o`, `*.so`, `*.a`, `*.dylib`, `.DS_Store`
- Removes macOS debug symbols: `*.dSYM`
- Restores project to original clean state

### 🔒 Safety Features
- Checks for required tools (GCC, make)
- Verifies project structure before proceeding
- Confirmation prompt for cleanup operations
- Graceful handling of Ctrl+C interruption
- Error checking with immediate exit on failure

### 🎨 User Experience
- Color-coded output (compatible with macOS Terminal)
- Clear success/warning/error indicators
- Progress information during builds
- File size information for built executables
- Helpful error messages

## Examples

### Basic Development Workflow

```bash
# Start development
./build.sh build debug

# Test the application
./build.sh run

# Make changes to code...

# Rebuild and test
./build.sh run

# When done, clean everything
./build.sh clean
```

### Preparing for Distribution

```bash
# Build optimized version
./build.sh build release

# Test the release build
./build.sh run

# Create distribution package
make dist

# Clean up
./build.sh clean
```

### Troubleshooting Build Issues

```bash
# Check current status
./build.sh status

# Clean and rebuild from scratch
./build.sh clean
./build.sh build debug

# Check system requirements
./build.sh help  # Shows system requirements
```

## What Gets Cleaned

When you run `./build.sh clean`, the script removes:

### Build Directories
- `bin/` - Compiled executables
- `obj/` - Object files
- `lib/` - Library files

### Temporary Files
- `*.o` - Object files
- `*.so` - Shared libraries  
- `*.a` - Static libraries
- `*.dylib` - macOS dynamic libraries
- `core`, `*.core` - Core dump files
- `.DS_Store` - macOS Finder metadata
- `*.dSYM` - macOS debug symbol directories
- `*.log` - Log files
- `*.tmp` - Temporary files

### Result
After cleaning, your project directory will be exactly as it was when:
- First cloned from Git
- Downloaded as a fresh copy
- Ready for version control operations
- Ready for distribution packaging

## System Requirements

- macOS (or other Unix-like system)
- GCC compiler (Xcode Command Line Tools)
- GNU Make utility
- Bash shell (zsh compatible)

### Installing Prerequisites on macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install
```

## Troubleshooting

### Script won't run
```bash
# Make sure it's executable
chmod +x build.sh

# Check if you're in the right directory
ls -la Makefile  # Should exist
```

### Build failures
```bash
# Check system requirements
./build.sh help

# Try a clean rebuild
./build.sh clean
./build.sh build debug
```

### Permission issues
```bash
# Make sure you own the project directory
ls -la .

# If needed, fix ownership
sudo chown -R $(whoami) .
```

## Integration with Git

The script respects `.gitignore` and helps maintain a clean repository:

```bash
# Before committing changes
./build.sh clean
git add .
git commit -m "Your changes"

# Before pushing to GitHub
./build.sh clean
git push
```

## Advanced Usage

### Scripting and Automation

```bash
#!/bin/bash
# Automated build and test script

set -e  # Exit on any error

# Clean and build
./build.sh clean
./build.sh build release

# Run tests (if you have them)
# ./run_tests.sh

echo "Build and test completed successfully!"
```

### Continuous Integration

The script can be used in CI/CD pipelines:

```bash
# In your CI script
./build.sh build release
./build.sh clean  # Clean up after build
```

## Contributing

If you need to modify the build script:

1. **Test thoroughly** on different scenarios
2. **Maintain compatibility** with macOS
3. **Keep the interactive mode** user-friendly
4. **Preserve the cleanup functionality** - it should always restore the original state
5. **Update this documentation** if you change the interface

## Support

For build script issues:
1. Check system requirements: `./build.sh help`
2. Try a clean rebuild: `./build.sh clean && ./build.sh build`
3. Check the project Makefile for any recent changes
4. Ensure you have the latest version of the script

The script is designed to be self-contained and should work on any macOS system with standard development tools installed.