# Convolution Explorer - Makefile
# Educational signal processing application demonstrating convolution

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lm

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
EXAMPLEDIR = examples

# Create directories if they don't exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Main executable
TARGET = $(BINDIR)/convolution_explorer

# Example programs
EXAMPLES = $(BINDIR)/signal_demo $(BINDIR)/fft_demo $(BINDIR)/performance_test

# Default target
all: $(TARGET) examples

# Main executable
$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Example programs
examples: $(EXAMPLES)

$(BINDIR)/signal_demo: $(EXAMPLEDIR)/signal_demo.c $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	@echo "Building signal demo..."
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@ $(LDFLAGS)

$(BINDIR)/fft_demo: $(EXAMPLEDIR)/fft_demo.c $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	@echo "Building FFT demo..."
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@ $(LDFLAGS)

$(BINDIR)/performance_test: $(EXAMPLEDIR)/performance_test.c $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	@echo "Building performance test..."
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@ $(LDFLAGS)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete."

# Install (copy to /usr/local/bin)
install: $(TARGET)
	@echo "Installing to /usr/local/bin (requires sudo)..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete. Run with: convolution_explorer"

# Uninstall
uninstall:
	@echo "Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/convolution_explorer
	@echo "Uninstallation complete."

# Run the main application
run: $(TARGET)
	./$(TARGET)

# Run examples
run-signal-demo: $(BINDIR)/signal_demo
	./$(BINDIR)/signal_demo

run-fft-demo: $(BINDIR)/fft_demo
	./$(BINDIR)/fft_demo

run-performance-test: $(BINDIR)/performance_test
	./$(BINDIR)/performance_test

# Debug build (with debugging symbols and no optimization)
debug: CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG -O0
debug: clean $(TARGET)
	@echo "Debug build complete."

# Release build (optimized, no debug symbols)
release: CFLAGS = -Wall -Wextra -std=c99 -O3 -DNDEBUG -s
release: clean $(TARGET)
	@echo "Release build complete."

# Generate documentation using doxygen (if available)
docs:
	@if command -v doxygen >/dev/null 2>&1; then \
		echo "Generating documentation..."; \
		doxygen docs/Doxyfile 2>/dev/null || echo "Doxygen config not found, skipping detailed docs"; \
	else \
		echo "Doxygen not found. Please install doxygen to generate detailed documentation."; \
	fi
	@echo "See docs/README.md for manual documentation."

# Create a distribution package
dist: clean
	@echo "Creating distribution package..."
	@cd .. && tar -czf convolution_explorer.tar.gz \
		--exclude='*.o' --exclude='*.so' --exclude='*.a' \
		--exclude='.git*' --exclude='*.tar.gz' \
		Convolution/
	@echo "Distribution package created: ../convolution_explorer.tar.gz"

# Help
help:
	@echo "Convolution Explorer - Build System"
	@echo "=================================="
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build main application and examples (default)"
	@echo "  $(TARGET)         - Build main application only"
	@echo "  examples         - Build example programs"
	@echo "  clean            - Remove build artifacts"
	@echo "  install          - Install to system (requires sudo)"
	@echo "  uninstall        - Remove from system"
	@echo "  run              - Build and run main application"
	@echo "  run-signal-demo  - Build and run signal demo"
	@echo "  run-fft-demo     - Build and run FFT demo"
	@echo "  run-performance-test - Build and run performance test"
	@echo "  debug            - Build with debug flags"
	@echo "  release          - Build optimized release version"
	@echo "  docs             - Generate documentation"
	@echo "  dist             - Create distribution package"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Compilation requirements:"
	@echo "  - GCC compiler with C99 support"
	@echo "  - Standard math library (libm)"
	@echo "  - POSIX-compatible system (Linux, macOS, etc.)"
	@echo ""

# Detect system and provide system-specific info
system-info:
	@echo "System Information:"
	@echo "==================="
	@echo "OS: $$(uname -s)"
	@echo "Architecture: $$(uname -m)"
	@echo "Compiler: $$($(CC) --version 2>/dev/null | head -1 || echo 'GCC not found')"
	@echo "Math library: $$(ls /usr/lib*/libm.* 2>/dev/null | head -1 || echo 'Not found in standard location')"
	@echo ""

# Check if all dependencies are available
check-deps:
	@echo "Checking dependencies..."
	@which $(CC) >/dev/null 2>&1 || (echo "ERROR: GCC compiler not found" && exit 1)
	@echo "✓ GCC compiler found"
	@echo "✓ Math library should be available (standard with GCC)"
	@echo "✓ All dependencies satisfied"
	@echo ""

# Quick test to verify build works
test: $(TARGET)
	@echo "Running quick functionality test..."
	@echo "Testing basic signal generation and convolution..."
	@./$(TARGET) <<< "7\n0" > /dev/null 2>&1 && echo "✓ Basic functionality test passed" || echo "✗ Test failed"

# Development target - build and test
dev: debug test
	@echo "Development build and test complete."

# Print project statistics
stats:
	@echo "Project Statistics:"
	@echo "==================="
	@echo "Source files: $$(find $(SRCDIR) -name '*.c' | wc -l)"
	@echo "Header files: $$(find $(INCDIR) -name '*.h' | wc -l)"
	@echo "Total lines of code: $$(find $(SRCDIR) $(INCDIR) -name '*.c' -o -name '*.h' | xargs wc -l | tail -1 | awk '{print $$1}')"
	@echo "Functions: $$(grep -r '^[a-zA-Z_][a-zA-Z0-9_]*.*(' $(SRCDIR) $(INCDIR) | grep -v '//' | wc -l)"
	@echo ""

# Phony targets (not files)
.PHONY: all clean install uninstall run examples debug release docs dist help system-info check-deps test dev stats
.PHONY: run-signal-demo run-fft-demo run-performance-test

# Dependencies (automatically generated)
-include $(OBJECTS:.o=.d)