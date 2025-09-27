#include "../include/convolution.h"
#include <time.h>

// Create a new signal structure
Signal* create_signal(int length, double sample_rate) {
    Signal *signal = (Signal*)malloc(sizeof(Signal));
    if (!signal) return NULL;
    
    signal->data = (double*)calloc(length, sizeof(double));
    if (!signal->data) {
        free(signal);
        return NULL;
    }
    
    signal->length = length;
    signal->sample_rate = sample_rate;
    signal->duration = (double)length / sample_rate;
    signal->type = SIGNAL_CUSTOM;
    strcpy(signal->name, "Untitled Signal");
    
    return signal;
}

// Free signal memory
void free_signal(Signal *signal) {
    if (signal) {
        if (signal->data) {
            free(signal->data);
        }
        free(signal);
    }
}

// Generate sine wave
Signal* generate_sine_wave(double frequency, double amplitude, double phase, 
                          double duration, double sample_rate) {
    int length = (int)(duration * sample_rate);
    Signal *signal = create_signal(length, sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_SINE;
    snprintf(signal->name, sizeof(signal->name), 
             "Sine Wave (%.1fHz, %.2fA)", frequency, amplitude);
    
    for (int i = 0; i < length; i++) {
        double t = (double)i / sample_rate;
        signal->data[i] = amplitude * sin(TWO_PI * frequency * t + phase);
    }
    
    return signal;
}

// Generate square wave
Signal* generate_square_wave(double frequency, double amplitude, 
                           double duration, double sample_rate) {
    int length = (int)(duration * sample_rate);
    Signal *signal = create_signal(length, sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_SQUARE;
    snprintf(signal->name, sizeof(signal->name), 
             "Square Wave (%.1fHz, %.2fA)", frequency, amplitude);
    
    for (int i = 0; i < length; i++) {
        double t = (double)i / sample_rate;
        double sine_val = sin(TWO_PI * frequency * t);
        signal->data[i] = amplitude * (sine_val >= 0 ? 1.0 : -1.0);
    }
    
    return signal;
}

// Generate triangle wave
Signal* generate_triangle_wave(double frequency, double amplitude, 
                             double duration, double sample_rate) {
    int length = (int)(duration * sample_rate);
    Signal *signal = create_signal(length, sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_TRIANGLE;
    snprintf(signal->name, sizeof(signal->name), 
             "Triangle Wave (%.1fHz, %.2fA)", frequency, amplitude);
    
    for (int i = 0; i < length; i++) {
        double t = (double)i / sample_rate;
        double phase = fmod(t * frequency, 1.0);
        
        if (phase < 0.5) {
            signal->data[i] = amplitude * (4.0 * phase - 1.0);
        } else {
            signal->data[i] = amplitude * (3.0 - 4.0 * phase);
        }
    }
    
    return signal;
}

// Generate sawtooth wave
Signal* generate_sawtooth_wave(double frequency, double amplitude, 
                             double duration, double sample_rate) {
    int length = (int)(duration * sample_rate);
    Signal *signal = create_signal(length, sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_SAWTOOTH;
    snprintf(signal->name, sizeof(signal->name), 
             "Sawtooth Wave (%.1fHz, %.2fA)", frequency, amplitude);
    
    for (int i = 0; i < length; i++) {
        double t = (double)i / sample_rate;
        double phase = fmod(t * frequency, 1.0);
        signal->data[i] = amplitude * (2.0 * phase - 1.0);
    }
    
    return signal;
}

// Generate white noise
Signal* generate_noise(double amplitude, double duration, double sample_rate) {
    int length = (int)(duration * sample_rate);
    Signal *signal = create_signal(length, sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_NOISE;
    snprintf(signal->name, sizeof(signal->name), 
             "White Noise (%.2fA)", amplitude);
    
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < length; i++) {
        // Generate random number between -1 and 1
        double random_val = 2.0 * ((double)rand() / RAND_MAX) - 1.0;
        signal->data[i] = amplitude * random_val;
    }
    
    return signal;
}

// Generate impulse response
Signal* generate_impulse(double amplitude, double delay, 
                        double duration, double sample_rate) {
    int length = (int)(duration * sample_rate);
    Signal *signal = create_signal(length, sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_IMPULSE;
    snprintf(signal->name, sizeof(signal->name), 
             "Impulse (%.2fA, %.3fs delay)", amplitude, delay);
    
    int delay_samples = (int)(delay * sample_rate);
    
    // Set all samples to 0 except the impulse
    for (int i = 0; i < length; i++) {
        signal->data[i] = 0.0;
    }
    
    // Set the impulse
    if (delay_samples >= 0 && delay_samples < length) {
        signal->data[delay_samples] = amplitude;
    }
    
    return signal;
}

// Generate Gaussian pulse
Signal* generate_gaussian_pulse(double amplitude, double sigma, double center, 
                               double duration, double sample_rate) {
    int length = (int)(duration * sample_rate);
    Signal *signal = create_signal(length, sample_rate);
    if (!signal) return NULL;
    
    signal->type = SIGNAL_GAUSSIAN;
    snprintf(signal->name, sizeof(signal->name), 
             "Gaussian Pulse (σ=%.3f, center=%.3fs)", sigma, center);
    
    int center_sample = (int)(center * sample_rate);
    
    for (int i = 0; i < length; i++) {
        double t = (double)(i - center_sample) / sample_rate;
        double exponent = -(t * t) / (2.0 * sigma * sigma);
        signal->data[i] = amplitude * exp(exponent);
    }
    
    return signal;
}

// Print signal information
void print_signal_info(const Signal *signal) {
    if (!signal) {
        printf("Signal is NULL\n");
        return;
    }
    
    printf("Signal Information:\n");
    printf("  Name: %s\n", signal->name);
    printf("  Type: ");
    
    switch (signal->type) {
        case SIGNAL_SINE: printf("Sine Wave\n"); break;
        case SIGNAL_SQUARE: printf("Square Wave\n"); break;
        case SIGNAL_TRIANGLE: printf("Triangle Wave\n"); break;
        case SIGNAL_SAWTOOTH: printf("Sawtooth Wave\n"); break;
        case SIGNAL_NOISE: printf("White Noise\n"); break;
        case SIGNAL_IMPULSE: printf("Impulse\n"); break;
        case SIGNAL_GAUSSIAN: printf("Gaussian Pulse\n"); break;
        case SIGNAL_CUSTOM: printf("Custom Signal\n"); break;
        default: printf("Unknown\n"); break;
    }
    
    printf("  Length: %d samples\n", signal->length);
    printf("  Sample Rate: %.1f Hz\n", signal->sample_rate);
    printf("  Duration: %.3f seconds\n", signal->duration);
    
    // Calculate basic statistics
    double min_val = signal->data[0];
    double max_val = signal->data[0];
    double sum = 0.0;
    
    for (int i = 0; i < signal->length; i++) {
        double val = signal->data[i];
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
        sum += val;
    }
    
    double mean = sum / signal->length;
    double variance = 0.0;
    
    for (int i = 0; i < signal->length; i++) {
        double diff = signal->data[i] - mean;
        variance += diff * diff;
    }
    variance /= signal->length;
    
    printf("  Range: [%.6f, %.6f]\n", min_val, max_val);
    printf("  Mean: %.6f\n", mean);
    printf("  Standard Deviation: %.6f\n", sqrt(variance));
    printf("\n");
}

// Normalize signal to [-1, 1] range
void normalize_signal(Signal *signal) {
    if (!signal || !signal->data) return;
    
    // Find min and max values
    double min_val = signal->data[0];
    double max_val = signal->data[0];
    
    for (int i = 1; i < signal->length; i++) {
        if (signal->data[i] < min_val) min_val = signal->data[i];
        if (signal->data[i] > max_val) max_val = signal->data[i];
    }
    
    // Avoid division by zero
    double range = max_val - min_val;
    if (range < 1e-10) return;
    
    // Normalize to [-1, 1]
    for (int i = 0; i < signal->length; i++) {
        signal->data[i] = 2.0 * (signal->data[i] - min_val) / range - 1.0;
    }
}

// Apply window function to signal
Signal* window_signal(const Signal *signal, const char *window_type) {
    if (!signal) return NULL;
    
    Signal *windowed = create_signal(signal->length, signal->sample_rate);
    if (!windowed) return NULL;
    
    // Copy original data
    memcpy(windowed->data, signal->data, signal->length * sizeof(double));
    windowed->type = signal->type;
    snprintf(windowed->name, sizeof(windowed->name), 
             "%s (%s windowed)", signal->name, window_type);
    
    // Apply window function
    for (int i = 0; i < signal->length; i++) {
        double window_val = 1.0; // Default: rectangular window
        
        if (strcmp(window_type, "hann") == 0 || strcmp(window_type, "hanning") == 0) {
            // Hann window
            window_val = 0.5 * (1.0 - cos(TWO_PI * i / (signal->length - 1)));
        } else if (strcmp(window_type, "hamming") == 0) {
            // Hamming window
            window_val = 0.54 - 0.46 * cos(TWO_PI * i / (signal->length - 1));
        } else if (strcmp(window_type, "blackman") == 0) {
            // Blackman window
            double a0 = 0.42, a1 = 0.5, a2 = 0.08;
            window_val = a0 - a1 * cos(TWO_PI * i / (signal->length - 1)) 
                           + a2 * cos(4.0 * PI * i / (signal->length - 1));
        }
        
        windowed->data[i] *= window_val;
    }
    
    return windowed;
}