# Contributing to Convolution Explorer

Thank you for your interest in contributing to Convolution Explorer! This project aims to provide an educational and interactive platform for understanding convolution in signal processing.

## Getting Started

### Prerequisites
- GCC compiler with C99 support
- Standard math library (`libm`)
- POSIX-compatible system (Linux, macOS, Unix)

### Building the Project
```bash
git clone https://github.com/anthonychambet/convolution-explorer.git
cd convolution-explorer
make
./bin/convolution_explorer
```

## How to Contribute

### Reporting Issues
- Use GitHub Issues to report bugs or suggest features
- Include system information (OS, compiler version)
- Provide steps to reproduce the issue
- Include sample code if relevant

### Code Contributions

#### Areas for Contribution
1. **New Signal Types**: Add more signal generators (chirp, AM/FM modulation, etc.)
2. **Visualization**: Improve ASCII plots or add graphical visualization
3. **Algorithms**: Implement other convolution methods (overlap-add, overlap-save)
4. **Performance**: Optimize existing algorithms
5. **Documentation**: Improve tutorials and examples
6. **Testing**: Add unit tests and validation

#### Code Style Guidelines
- **Language**: C99 standard
- **Naming**: Use descriptive variable and function names
- **Comments**: Document complex algorithms and mathematical concepts
- **Memory**: Always pair malloc() with free()
- **Error Handling**: Check return values and handle edge cases

#### Example Contribution - Adding a New Signal Type
```c
// In signal_generation.c
Signal* generate_chirp(double f0, double f1, double amplitude, 
                      double duration, double sample_rate) {
    Signal *signal = create_signal((int)(duration * sample_rate), sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_CHIRP;
    snprintf(signal->name, sizeof(signal->name), 
             "Chirp (%.1f-%.1fHz, %.2fA)", f0, f1, amplitude);
    
    for (int i = 0; i < signal->length; i++) {
        double t = (double)i / sample_rate;
        double freq = f0 + (f1 - f0) * t / duration;
        signal->data[i] = amplitude * sin(2 * PI * freq * t);
    }
    
    return signal;
}
```

### Pull Request Process
1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/new-signal-type`
3. **Make** your changes following the code style
4. **Test** your changes: `make clean && make && make test`
5. **Document** your changes in comments and README if needed
6. **Commit** with clear messages: `git commit -m "Add chirp signal generator"`
7. **Push** to your fork: `git push origin feature/new-signal-type`
8. **Create** a Pull Request with description of changes

### Testing
Before submitting:
```bash
# Build and test
make clean
make
make test

# Check for memory leaks (if valgrind is available)
valgrind --leak-check=full ./bin/convolution_explorer

# Test different demos
echo "1\n0" | ./bin/convolution_explorer  # Basic demo
echo "7\n0" | ./bin/convolution_explorer  # Tutorial
```

## Educational Focus

This project prioritizes educational value:
- **Clarity over Performance**: Code should be readable and understandable
- **Mathematical Accuracy**: Implement algorithms faithfully to theory
- **Interactive Learning**: Features should help users understand concepts
- **Documentation**: Explain both the "how" and "why" of implementations

## Project Structure

```
convolution-explorer/
├── src/                 # Source code
│   ├── main.c          # Interactive application
│   ├── signal_generation.c
│   ├── convolution_ops.c
│   └── visualization.c
├── include/            # Header files
├── examples/           # Example programs
├── docs/              # Documentation
└── Makefile           # Build system
```

## Ideas for Future Contributions

### New Features
- **2D Convolution**: Image processing examples
- **Real-time Processing**: Audio input/output
- **Advanced Filters**: Butterworth, Chebyshev, elliptic
- **Windowing Functions**: More window types
- **File I/O**: WAV file support
- **Web Interface**: Browser-based version

### Educational Enhancements
- **Animated Tutorials**: Step-by-step convolution visualization
- **Interactive Parameters**: Real-time parameter adjustment
- **Comparison Tools**: Side-by-side algorithm comparison
- **Quiz Mode**: Interactive learning assessments

### Technical Improvements
- **Multithreading**: Parallel processing for large signals
- **SIMD Optimization**: Vector instructions for performance
- **GPU Acceleration**: CUDA/OpenCL support
- **Arbitrary Precision**: High-precision arithmetic
- **Memory Optimization**: Streaming for huge datasets

## Code Review Criteria

Contributions are evaluated on:
- **Correctness**: Does it work as intended?
- **Educational Value**: Does it help users learn?
- **Code Quality**: Is it well-written and documented?
- **Compatibility**: Does it work across platforms?
- **Performance**: Is it reasonably efficient?

## Questions?

Feel free to:
- Open an issue for questions
- Start a discussion for ideas
- Contact the maintainer for guidance

## Recognition

Contributors will be acknowledged in:
- README.md contributors section
- Git commit history
- Release notes for significant contributions

Thank you for helping make signal processing education more accessible!