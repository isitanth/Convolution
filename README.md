# Convolution explorer

An interactive application for understanding convolution in signal processing through visual and mathematical analysis.

## Overview

Convolution Explorer is a C-based application that demonstrates how convolution works in both time and frequency domains. It provides visualizations, multiple demo modes, and hands-on learning experiences to help understand this fundamental operation in signal processing.

## What is convolution?

Convolution is a mathematical operation that combines two signals to produce a third signal. It's defined as:

```
(x * h)[n] = Σ x[k] × h[n-k]
			 k
```
Where:
- `x[n]` is the input signal
- `h[n]` is the kernel/filter
- `*` denotes convolution (not multiplication)
- The output combines information from both signals
Or, in proper mathematical notation:

$$
y[n] = (x * h)[n] = \sum_{k=-\infty}^{+\infty} x[k] \cdot h[n-k]
$$

In practice, when signals are finite and represented as arrays of length N (for x) and M (for h), the sum is bounded:

$$
y[n] = \sum_{k=0}^{N-1} x[k] \cdot h[n-k], \quad 0 \leq n \leq N+M-2
$$

### Applications

Convolution is nice for:
- **Digital Signal Processing**: Filtering, noise reduction
- **Image Processing**: Blur, sharpen, edge detection
- **System Analysis**: Characterizing linear time-invariant systems
- **Audio Processing**: Reverb, echo effects
- **Neural Networks**: Convolutional layers for feature extraction

## Features

### Interactive demos
1. **Basic Convolution**: Compare linear vs circular convolution
2. **Signal Filtering**: See how convolution removes noise
3. **Frequency Analysis**: Explore time-frequency relationships
4. **System Response**: Understand impulse and step responses
5. **Custom Signals**: Create and convolve your own signals
6. **Performance Comparison**: Direct vs FFT-based convolution

### Signal types
- Sine waves (pure tones)
- Square waves (digital signals)
- Triangle waves (linear ramps)
- Sawtooth waves (periodic ramps)
- White noise (random signals)
- Gaussian pulses (smooth bumps)
- Impulse responses (system characterization)

### Analysis tools
- ASCII-based plotting (no external dependencies)
- Fast Fourier Transform (FFT) implementation
- Frequency domain visualization
- Signal statistics and properties
- Cross-correlation analysis

### Algorithms
- **Direct Convolution**: O(N×M) time complexity
- **Circular Convolution**: For periodic signals
- **FFT Convolution**: O(N log N) for large signals
- **Custom FFT**: No external library dependencies

## Quick start

### Prerequisites
- GCC compiler with C99 support
- Standard math library (`libm`)
- POSIX-compatible system (Linux, macOS, Unix)

### Building
```bash
# Clone or download the project
cd Convolution

# Build everything
make

# Or just build the main application
make bin/convolution_explorer

# Run the application
make run
# or
./bin/convolution_explorer
```

### Example session
```
╔════════════════════════════════════════════════╗
║              CONVOLUTION EXPLORER              ║
║     Understanding Signal Processing through    ║
║        Visual and Mathematical Analysis        ║
╚════════════════════════════════════════════════╝

1. Basic Convolution Demo (Linear vs Circular)
2. Signal Filtering Examples
3. Frequency Domain Analysis
4. System Impulse Response
5. Custom Signal Generator
6. Performance Comparison (Direct vs FFT)
7. Interactive Tutorial
0. Exit

Enter your choice (0-7): 1
```

## Project structure

```
Convolution/
├── README.md           # This file
├── Makefile           # Build system
├── include/
│   └── convolution.h  # Main header with all declarations
├── src/
│   ├── main.c                # Interactive application
│   ├── signal_generation.c  # Signal creation functions
│   ├── convolution_ops.c     # Core convolution algorithms
│   └── visualization.c       # ASCII plotting and display
├── examples/          # Example programs (created by make)
├── docs/             # Additional documentation
├── bin/              # Compiled executables (created by make)
└── obj/              # Object files (created by make)
```

## Mathematical background

### Discrete convolution

For two discrete signals x[n] and h[n], convolution is:
```
y[n] = (x * h)[n] = Σ x[k] × h[n-k]
```

This can be understood as:
1. **Flip** the kernel h[n] → h[-n]
2. **Shift** by n samples → h[n-k]
3. **Multiply** element-wise with x[k]
4. **Sum** all products

### Linear vs circular convolution

**Linear convolution:**
- Output length: N + M - 1
- Assumes zero-padding outside signal bounds
- Used for filtering and system analysis

**Circular convolution:**
- Output length: max(N, M)
- Wraps around signal boundaries
- Equivalent to FFT-based multiplication

### Frequency domain relationship

The Convolution Theorem states:
```
F{x * h} = F{x} × F{h}
```

Where F{} denotes the Fourier transform. This means:
- Convolution in time ↔ Multiplication in frequency
- FFT convolution exploits this for efficiency

## Demo descriptions

### 1. Basic convolution demo
Demonstrates the fundamental difference between linear and circular convolution using a sine wave and rectangular pulse.

**Learning objectives:**
- Understand convolution mechanics
- See output length differences
- Visualize time-domain effects

### 2. Signal filtering
Shows how convolution implements digital filters using a composite noisy signal.

**Learning objectives:**
- Convolution as filtering
- Low-pass filter design
- Noise reduction techniques

### 3. Frequency analysis
Explores the relationship between time and frequency domains.

**Learning Objectives:**
- Convolution theorem
- FFT implementation
- Spectral analysis

### 4. System response
Demonstrates how convolution characterizes linear time-invariant (LTI) systems.

**Learning Objectives:**
- Impulse response
- Step response
- System identification

### 5. Custom signals
Interactive signal generator for hands-on experimentation.

**Learning Objectives:**
- Signal properties
- Convolution effects
- Creative exploration

### 6. Performance comparison
Benchmarks different convolution algorithms.

**Learning Objectives:**
- Algorithm complexity
- Implementation trade-offs
- Computational efficiency

### 7. Interactive tutorial
Step-by-step walkthrough of convolution fundamentals.

**Learning Objectives:**
- Mathematical foundations
- Visual understanding
- Practical applications

## Build options

```bash
# Standard build
make                    # Build everything
make all               # Same as make

# Development
make debug             # Debug build with symbols
make release           # Optimized release build
make test              # Quick functionality test

# Utilities
make clean             # Remove build artifacts
make help              # Show all available targets
make stats             # Project statistics
make system-info       # System compatibility check

# Installation
make install           # Install to /usr/local/bin (requires sudo)
make uninstall         # Remove from system
```

## Advanced usage

### Creating custom signals
```c
// Example: Create a custom signal
Signal *my_signal = create_signal(1000, 44100.0);
for (int i = 0; i < my_signal->length; i++) {
	double t = (double)i / my_signal->sample_rate;
	my_signal->data[i] = sin(2 * PI * 440 * t); // 440 Hz sine wave
}
```

### FFT-based convolution
```c
// Automatically chooses best method based on signal size
Signal *result = convolve_fft(signal1, signal2);
```

### Signal analysis
```c
// Compute frequency spectrum
FFTResult *spectrum = compute_fft(signal);
// spectrum->magnitude contains the magnitude spectrum
// spectrum->phase contains the phase spectrum
```

## Educational value

This project is designed for:
- **Students** learning digital signal processing
- **Engineers** reviewing convolution concepts
- **Researchers** prototyping convolution applications
- **Educators** teaching signal processing fundamentals

### Key concepts covered
1. **Mathematical Foundations**: Discrete convolution formula
2. **Implementation**: Direct and FFT-based algorithms
3. **Applications**: Filtering, system analysis, signal processing
4. **Visualization**: Time and frequency domain representations
5. **Performance**: Algorithm complexity and optimization

## Troubleshooting

### Compilation issues
```bash
# Check dependencies
make check-deps

# Try debug build for more information
make debug

# Check system compatibility
make system-info
```

### Runtime issues
- Ensure you have sufficient memory for large signals
- FFT requires signal lengths that are powers of 2 for optimal performance
- Some visualizations may not display correctly on narrow terminals

## Contributing

This is an educational project. To extend or modify:

1. **Add New Signal Types**: Extend `signal_generation.c`
2. **New Algorithms**: Add to `convolution_ops.c`
3. **Better Visualization**: Enhance `visualization.c`
4. **More Demos**: Expand `main.c`

### Code style
- C99 standard
- Clear variable names
- Comprehensive comments
- Memory management (malloc/free)
- Error checking

## Performance notes

- **Small signals (< 256 samples)**: Direct convolution is faster
- **Large signals (> 512 samples)**: FFT convolution is faster
- **Memory usage**: Linear in signal size
- **FFT size**: Automatically padded to next power of 2

## License

This is an educational project intended for learning purposes. Feel free to use, modify, and distribute for educational use.

## References

1. Online community resources (technical forums, Reddit discussions, open-source contributions, YouTube tutorials, ZLibrary books).  
2. Academic background in engineering, including DUT (French university diploma) coursework.  
3. Popular science and AI knowledge-sharing materials.  

