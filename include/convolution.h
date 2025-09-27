#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>

// Constants
#define MAX_SIGNAL_LENGTH 4096
#define PI 3.14159265359
#define TWO_PI 6.28318530718

// Signal types
typedef enum {
    SIGNAL_SINE,
    SIGNAL_SQUARE,
    SIGNAL_TRIANGLE,
    SIGNAL_SAWTOOTH,
    SIGNAL_NOISE,
    SIGNAL_IMPULSE,
    SIGNAL_GAUSSIAN,
    SIGNAL_CUSTOM
} SignalType;

// Signal structure
typedef struct {
    double *data;           // Signal samples
    int length;            // Number of samples
    double sample_rate;    // Sampling rate in Hz
    double duration;       // Duration in seconds
    SignalType type;       // Type of signal
    char name[64];         // Signal name for display
} Signal;

// Complex number for FFT
typedef struct {
    double real;
    double imag;
} Complex;

// FFT Result structure
typedef struct {
    Complex *data;         // Complex frequency domain data
    double *magnitude;     // Magnitude spectrum
    double *phase;         // Phase spectrum
    double *frequency;     // Frequency bins
    int length;           // Number of frequency bins
} FFTResult;

// Visualization structure
typedef struct {
    int width;
    int height;
    double *time_axis;
    double *freq_axis;
    int show_time_domain;
    int show_freq_domain;
    int show_convolution;
} Visualization;

// Function prototypes

// Signal generation
Signal* create_signal(int length, double sample_rate);
void free_signal(Signal *signal);
Signal* generate_sine_wave(double frequency, double amplitude, double phase, 
                          double duration, double sample_rate);
Signal* generate_square_wave(double frequency, double amplitude, 
                           double duration, double sample_rate);
Signal* generate_triangle_wave(double frequency, double amplitude, 
                             double duration, double sample_rate);
Signal* generate_sawtooth_wave(double frequency, double amplitude, 
                             double duration, double sample_rate);
Signal* generate_noise(double amplitude, double duration, double sample_rate);
Signal* generate_impulse(double amplitude, double delay, 
                        double duration, double sample_rate);
Signal* generate_gaussian_pulse(double amplitude, double sigma, double center, 
                               double duration, double sample_rate);

// Convolution operations
Signal* convolve(const Signal *signal1, const Signal *signal2);
Signal* convolve_circular(const Signal *signal1, const Signal *signal2);
Signal* convolve_fft(const Signal *signal1, const Signal *signal2);

// FFT operations
FFTResult* compute_fft(const Signal *signal);
void free_fft_result(FFTResult *result);
void fft_recursive(Complex *data, int n);
void ifft_recursive(Complex *data, int n);

// Utility functions
void print_signal_info(const Signal *signal);
void save_signal_to_file(const Signal *signal, const char *filename);
Signal* load_signal_from_file(const char *filename);
void normalize_signal(Signal *signal);
Signal* window_signal(const Signal *signal, const char *window_type);

// Visualization functions
int init_visualization(int width, int height);
void plot_signal(const Signal *signal, const char *title, int subplot);
void plot_fft(const FFTResult *fft, const char *title, int subplot);
void plot_convolution_demo(const Signal *input, const Signal *kernel, 
                          const Signal *output);
void cleanup_visualization(void);

// Interactive demo functions
void run_interactive_demo(void);
void demo_lowpass_filter(void);
void demo_edge_detection(void);
void demo_echo_effect(void);
void demo_system_response(void);

#endif // CONVOLUTION_H