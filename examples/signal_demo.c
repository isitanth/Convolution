/**
 * Signal Demo - Simple example of using the Convolution library
 * 
 * This program demonstrates basic signal generation and convolution
 * operations without the interactive interface.
 */

#include "../include/convolution.h"
#include <time.h>

int main() {
    printf("=== Convolution Library Demo ===\n\n");
    
    // Create sample parameters
    double sample_rate = 1000.0;  // 1 kHz
    double duration = 1.0;        // 1 second
    
    printf("Creating signals...\n");
    
    // Create a 10 Hz sine wave
    Signal *sine_wave = generate_sine_wave(10.0, 1.0, 0.0, duration, sample_rate);
    print_signal_info(sine_wave);
    
    // Create a simple low-pass filter (moving average)
    Signal *filter = create_signal(21, sample_rate); // 21-point filter
    for (int i = 0; i < filter->length; i++) {
        filter->data[i] = 1.0 / 21.0; // Normalized
    }
    strcpy(filter->name, "21-point Moving Average Filter");
    print_signal_info(filter);
    
    printf("Performing convolution...\n");
    
    // Convolve the sine wave with the filter
    Signal *filtered_sine = convolve(sine_wave, filter);
    print_signal_info(filtered_sine);
    
    printf("Frequency analysis...\n");
    
    // Analyze frequency content
    FFTResult *original_fft = compute_fft(sine_wave);
    FFTResult *filtered_fft = compute_fft(filtered_sine);
    
    if (original_fft && filtered_fft) {
        // Find peak frequencies
        int original_peak = 0, filtered_peak = 0;
        double original_max = 0.0, filtered_max = 0.0;
        
        int half_length = original_fft->length / 2;
        for (int i = 1; i < half_length; i++) {
            if (original_fft->magnitude[i] > original_max) {
                original_max = original_fft->magnitude[i];
                original_peak = i;
            }
            if (filtered_fft->magnitude[i] > filtered_max) {
                filtered_max = filtered_fft->magnitude[i];
                filtered_peak = i;
            }
        }
        
        printf("\nFrequency Analysis Results:\n");
        printf("Original signal peak: %.1f Hz (magnitude: %.3f)\n", 
               original_fft->frequency[original_peak], original_max);
        printf("Filtered signal peak: %.1f Hz (magnitude: %.3f)\n", 
               filtered_fft->frequency[filtered_peak], filtered_max);
        printf("Attenuation: %.1f dB\n", 
               20 * log10(filtered_max / original_max));
        
        free_fft_result(original_fft);
        free_fft_result(filtered_fft);
    }
    
    printf("\nTesting different convolution methods...\n");
    
    // Compare direct vs FFT convolution
    clock_t start = clock();
    Signal *direct_result = convolve(sine_wave, filter);
    clock_t direct_time = clock() - start;
    
    start = clock();
    Signal *fft_result = convolve_fft(sine_wave, filter);
    clock_t fft_time = clock() - start;
    
    printf("Direct convolution time: %.3f ms\n", 
           ((double)direct_time / CLOCKS_PER_SEC) * 1000.0);
    printf("FFT convolution time: %.3f ms\n", 
           ((double)fft_time / CLOCKS_PER_SEC) * 1000.0);
    
    // Check if results are similar
    double max_diff = 0.0;
    int min_length = (direct_result->length < fft_result->length) ? 
                     direct_result->length : fft_result->length;
    
    for (int i = 0; i < min_length; i++) {
        double diff = fabs(direct_result->data[i] - fft_result->data[i]);
        if (diff > max_diff) max_diff = diff;
    }
    
    printf("Maximum difference between methods: %.2e\n", max_diff);
    printf("Results are %s\n", (max_diff < 1e-10) ? "identical" : "slightly different");
    
    printf("\nSaving results to files...\n");
    
    // Save signals to CSV files for external analysis
    save_signal_to_file(sine_wave, "original_sine.csv");
    save_signal_to_file(filtered_sine, "filtered_sine.csv");
    save_signal_to_file(filter, "filter_response.csv");
    
    printf("\nGenerating different signal types...\n");
    
    // Demonstrate different signal types
    Signal *signals[] = {
        generate_square_wave(10.0, 1.0, 0.5, sample_rate),
        generate_triangle_wave(10.0, 1.0, 0.5, sample_rate),
        generate_sawtooth_wave(10.0, 1.0, 0.5, sample_rate),
        generate_noise(0.5, 0.5, sample_rate),
        generate_gaussian_pulse(1.0, 0.05, 0.25, 0.5, sample_rate),
        generate_impulse(1.0, 0.1, 0.5, sample_rate)
    };
    
    const char *signal_names[] = {
        "Square Wave", "Triangle Wave", "Sawtooth Wave", 
        "White Noise", "Gaussian Pulse", "Impulse"
    };
    
    int num_signals = sizeof(signals) / sizeof(signals[0]);
    
    for (int i = 0; i < num_signals; i++) {
        if (signals[i]) {
            printf("\n%s:\n", signal_names[i]);
            print_signal_info(signals[i]);
        }
    }
    
    printf("\nTesting circular convolution...\n");
    
    // Test circular convolution with a simple example
    Signal *x = create_signal(4, 1.0);
    double x_vals[] = {1, 2, 3, 4};
    memcpy(x->data, x_vals, 4 * sizeof(double));
    strcpy(x->name, "Test signal x");
    
    Signal *h = create_signal(3, 1.0);
    double h_vals[] = {1, 0.5, 0.25};
    memcpy(h->data, h_vals, 3 * sizeof(double));
    strcpy(h->name, "Test filter h");
    
    Signal *linear_conv = convolve(x, h);
    Signal *circular_conv = convolve_circular(x, h);
    
    printf("\nLinear convolution result:\n");
    printf("Length: %d, Values: ", linear_conv->length);
    for (int i = 0; i < linear_conv->length; i++) {
        printf("%.3f ", linear_conv->data[i]);
    }
    printf("\n");
    
    printf("Circular convolution result:\n");
    printf("Length: %d, Values: ", circular_conv->length);
    for (int i = 0; i < circular_conv->length; i++) {
        printf("%.3f ", circular_conv->data[i]);
    }
    printf("\n");
    
    printf("\nDemo complete!\n");
    printf("\nFiles created:\n");
    printf("  - original_sine.csv\n");
    printf("  - filtered_sine.csv\n");
    printf("  - filter_response.csv\n");
    printf("\nThese files can be imported into spreadsheet applications\n");
    printf("or plotting tools like Python/matplotlib, MATLAB, or GNU Octave.\n");
    
    // Cleanup
    free_signal(sine_wave);
    free_signal(filter);
    free_signal(filtered_sine);
    free_signal(direct_result);
    free_signal(fft_result);
    free_signal(x);
    free_signal(h);
    free_signal(linear_conv);
    free_signal(circular_conv);
    
    for (int i = 0; i < num_signals; i++) {
        if (signals[i]) {
            free_signal(signals[i]);
        }
    }
    
    return 0;
}