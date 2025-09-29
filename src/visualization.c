#include "../include/convolution.h"

// ASCII plotting characters
#define PLOT_CHAR '*'
#define AXIS_CHAR '|'
#define HAXIS_CHAR '-'
#define ZERO_CHAR '+'

// Simple ASCII visualization for signals
void plot_signal_ascii(const Signal *signal, int width, int height) {
    if (!signal || !signal->data || width < 10 || height < 5) return;
    
    printf("\n=== %s ===\n", signal->name);
    printf("Length: %d samples, Sample Rate: %.1f Hz, Duration: %.3f s\n", 
           signal->length, signal->sample_rate, signal->duration);
    
    // Find min/max values for scaling
    double min_val = signal->data[0];
    double max_val = signal->data[0];
    for (int i = 1; i < signal->length; i++) {
        if (signal->data[i] < min_val) min_val = signal->data[i];
        if (signal->data[i] > max_val) max_val = signal->data[i];
    }
    
    // Handle edge case where signal is constant
    if (fabs(max_val - min_val) < 1e-10) {
        max_val += 0.1;
        min_val -= 0.1;
    }
    
    printf("Range: [%.6f, %.6f]\n\n", min_val, max_val);
    
    // Create plot buffer
    char **plot = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        plot[i] = (char*)malloc((width + 1) * sizeof(char));
        memset(plot[i], ' ', width);
        plot[i][width] = '\0';
    }
    
    // Draw y-axis
    int zero_line = (int)(height * (max_val) / (max_val - min_val));
    if (zero_line >= 0 && zero_line < height) {
        for (int x = 0; x < width; x++) {
            plot[zero_line][x] = HAXIS_CHAR;
        }
        plot[zero_line][0] = ZERO_CHAR;
    }
    
    // Plot signal data
    int samples_per_point = (signal->length > width) ? signal->length / width : 1;
    int points_per_sample = (signal->length < width) ? width / signal->length : 1;
    
    for (int x = 0; x < width && x < signal->length; x++) {
        int sample_idx;
        double value;
        
        if (samples_per_point > 1) {
            // Downsample: average multiple samples per plot point
            sample_idx = x * samples_per_point;
            value = 0.0;
            int count = 0;
            for (int i = 0; i < samples_per_point && (sample_idx + i) < signal->length; i++) {
                value += signal->data[sample_idx + i];
                count++;
            }
            value /= count;
        } else {
            // Direct mapping or interpolation
            sample_idx = x / points_per_sample;
            if (sample_idx >= signal->length) sample_idx = signal->length - 1;
            value = signal->data[sample_idx];
        }
        
        // Map value to y coordinate
        int y = (int)((height - 1) * (max_val - value) / (max_val - min_val));
        if (y >= 0 && y < height) {
            plot[y][x] = PLOT_CHAR;
        }
    }
    
    // Print the plot
    printf("  %.3f |", max_val);
    for (int x = 0; x < width; x++) printf("-");
    printf("\n");
    
    for (int y = 0; y < height; y++) {
        if (y == height/2) {
            printf("  %.3f |%s\n", (max_val + min_val)/2, plot[y]);
        } else {
            printf("        |%s\n", plot[y]);
        }
    }
    
    printf("  %.3f |", min_val);
    for (int x = 0; x < width; x++) printf("-");
    printf("\n");
    
    printf("        0");
    for (int x = 1; x < width; x++) {
        if (x % 10 == 0) printf("%d", x/10 % 10);
        else printf(" ");
    }
    printf("\n\n");
    
    // Free plot buffer
    for (int i = 0; i < height; i++) {
        free(plot[i]);
    }
    free(plot);
}

// Plot FFT magnitude spectrum
void plot_fft_ascii(const FFTResult *fft, int width, int height, int show_phase) {
    if (!fft || width < 10 || height < 5) return;
    
    printf("\n=== FFT Magnitude Spectrum ===\n");
    
    // We'll only plot the positive frequencies (first half of FFT)
    int half_length = fft->length / 2;
    
    // Find max magnitude for scaling
    double max_mag = 0.0;
    for (int i = 0; i < half_length; i++) {
        if (fft->magnitude[i] > max_mag) {
            max_mag = fft->magnitude[i];
        }
    }
    
    if (max_mag < 1e-10) {
        printf("No significant frequency content detected.\n");
        return;
    }
    
    printf("Max Magnitude: %.6f\n", max_mag);
    printf("Frequency Resolution: %.2f Hz\n\n", 
           fabs(fft->frequency[1] - fft->frequency[0]));
    
    // Create plot buffer
    char **plot = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        plot[i] = (char*)malloc((width + 1) * sizeof(char));
        memset(plot[i], ' ', width);
        plot[i][width] = '\0';
    }
    
    // Plot magnitude data
    for (int x = 0; x < width && x < half_length; x++) {
        int freq_idx = (x * half_length) / width;
        double magnitude = fft->magnitude[freq_idx];
        
        // Map magnitude to y coordinate (inverted for display)
        int y = height - 1 - (int)((height - 1) * magnitude / max_mag);
        if (y >= 0 && y < height) {
            plot[y][x] = PLOT_CHAR;
        }
    }
    
    // Print the plot
    printf("  %.3f |", max_mag);
    for (int x = 0; x < width; x++) printf("-");
    printf("\n");
    
    for (int y = 0; y < height; y++) {
        if (y == height/4) {
            printf("  %.3f |%s\n", max_mag * 3/4, plot[y]);
        } else if (y == height/2) {
            printf("  %.3f |%s\n", max_mag / 2, plot[y]);
        } else if (y == 3*height/4) {
            printf("  %.3f |%s\n", max_mag / 4, plot[y]);
        } else {
            printf("        |%s\n", plot[y]);
        }
    }
    
    printf("  0.000 |");
    for (int x = 0; x < width; x++) printf("-");
    printf("\n");
    
    // Frequency axis labels
    double max_freq = fft->frequency[half_length - 1];
    printf("       0Hz");
    for (int x = 10; x < width; x += 10) {
        double freq = (x * max_freq) / width;
        printf("   %.0fHz", freq);
    }
    printf("\n\n");
    
    // Show phase if requested
    if (show_phase) {
        printf("=== FFT Phase Spectrum ===\n");
        
        // Find significant phase values (where magnitude is substantial)
        for (int i = 0; i < half_length && i < 20; i++) { // Show first 20 bins
            if (fft->magnitude[i] > max_mag * 0.1) { // Only show if magnitude > 10% of max
                printf("Bin %2d: Freq=%.1f Hz, Mag=%.4f, Phase=%.2f rad (%.1f°)\n", 
                       i, fft->frequency[i], fft->magnitude[i], 
                       fft->phase[i], fft->phase[i] * 180.0 / PI);
            }
        }
        printf("\n");
    }
    
    // Free plot buffer
    for (int i = 0; i < height; i++) {
        free(plot[i]);
    }
    free(plot);
}

// Simple implementation of visualization functions for compatibility
int init_visualization(int width, int height) {
    printf("ASCII Visualization initialized (%d x %d)\n", width, height);
    printf("Note: For better graphics, consider installing SDL2 or similar library.\n\n");
    return 1; // Success
}

void plot_signal(const Signal *signal, const char *title, int subplot) {
    printf("\n[Subplot %d] %s\n", subplot, title ? title : "Signal Plot");
    plot_signal_ascii(signal, 100, 25); // Increased to 100 characters wide, 25 lines high
}

void plot_fft(const FFTResult *fft, const char *title, int subplot) {
    printf("\n[Subplot %d] %s\n", subplot, title ? title : "FFT Plot");
    plot_fft_ascii(fft, 100, 25, 1); // Increased to 100 characters wide, 25 lines high
}

void plot_convolution_demo(const Signal *input, const Signal *kernel, const Signal *output) {
    printf("\n===============================================\n");
    printf("            CONVOLUTION DEMONSTRATION\n");
    printf("===============================================\n");
    
    plot_signal(input, "Input Signal", 1);
    plot_signal(kernel, "Kernel/Filter", 2);
    plot_signal(output, "Convolution Result", 3);
    
    printf("Mathematical Operation: output[n] = Σ input[k] × kernel[n-k]\n");
    printf("Output length: %d (input: %d + kernel: %d - 1)\n\n", 
           output->length, input->length, kernel->length);
}

void cleanup_visualization(void) {
    printf("Visualization cleanup complete.\n");
}

// Enhanced plotting with statistics
void plot_signal_detailed(const Signal *signal) {
    if (!signal) return;
    
    plot_signal_ascii(signal, 120, 30); // Increased to 120 characters wide, 30 lines high
    
    // Calculate and display statistics
    double sum = 0.0, sum_sq = 0.0;
    double min_val = signal->data[0], max_val = signal->data[0];
    
    for (int i = 0; i < signal->length; i++) {
        double val = signal->data[i];
        sum += val;
        sum_sq += val * val;
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    
    double mean = sum / signal->length;
    double variance = (sum_sq / signal->length) - (mean * mean);
    double rms = sqrt(sum_sq / signal->length);
    double peak_to_peak = max_val - min_val;
    
    printf("Signal Statistics:\n");
    printf("  Mean (DC): %.6f\n", mean);
    printf("  RMS: %.6f\n", rms);
    printf("  Peak-to-Peak: %.6f\n", peak_to_peak);
    printf("  Variance: %.6f\n", variance);
    printf("  Std Dev: %.6f\n", sqrt(variance));
    printf("\n");
}

// Compare two signals side by side
void plot_signals_comparison(const Signal *sig1, const Signal *sig2, const char *title) {
    printf("\n=== %s ===\n", title ? title : "Signal Comparison");
    
    if (!sig1 || !sig2) {
        printf("Error: One or both signals are NULL\n");
        return;
    }
    
    printf("Signal 1: %s\n", sig1->name);
    printf("Signal 2: %s\n\n", sig2->name);
    
    // Plot both signals with ASCII
    plot_signal_ascii(sig1, 100, 20); // Increased resolution for comparison plots
    plot_signal_ascii(sig2, 100, 20);
    
    // Compare basic properties
    printf("Comparison:\n");
    printf("  Length: %d vs %d samples\n", sig1->length, sig2->length);
    printf("  Sample Rate: %.1f vs %.1f Hz\n", sig1->sample_rate, sig2->sample_rate);
    printf("  Duration: %.3f vs %.3f seconds\n", sig1->duration, sig2->duration);
    
    // Calculate correlation if same length
    if (sig1->length == sig2->length) {
        double correlation = 0.0;
        double sum1 = 0.0, sum2 = 0.0, sum1sq = 0.0, sum2sq = 0.0;
        
        for (int i = 0; i < sig1->length; i++) {
            sum1 += sig1->data[i];
            sum2 += sig2->data[i];
            sum1sq += sig1->data[i] * sig1->data[i];
            sum2sq += sig2->data[i] * sig2->data[i];
            correlation += sig1->data[i] * sig2->data[i];
        }
        
        double mean1 = sum1 / sig1->length;
        double mean2 = sum2 / sig2->length;
        double std1 = sqrt(sum1sq / sig1->length - mean1 * mean1);
        double std2 = sqrt(sum2sq / sig2->length - mean2 * mean2);
        
        if (std1 > 1e-10 && std2 > 1e-10) {
            correlation = (correlation / sig1->length - mean1 * mean2) / (std1 * std2);
            printf("  Cross-correlation: %.6f\n", correlation);
        }
    }
    printf("\n");
}

// Show spectrogram-style representation (simplified)
void show_signal_spectrogram(const Signal *signal, int window_size) {
    if (!signal || window_size <= 0) return;
    
    printf("\n=== Spectrogram-style Analysis: %s ===\n", signal->name);
    printf("Window size: %d samples\n\n", window_size);
    
    int num_windows = (signal->length - window_size) / (window_size / 2) + 1;
    if (num_windows < 1) {
        printf("Signal too short for spectrogram analysis.\n");
        return;
    }
    
    printf("Time windows: %d\n", num_windows);
    printf("Frequency analysis per window:\n\n");
    
    for (int w = 0; w < num_windows && w < 10; w++) { // Limit to 10 windows for display
        int start_idx = w * (window_size / 2);
        if (start_idx + window_size > signal->length) break;
        
        // Create windowed signal
        Signal windowed_signal;
        windowed_signal.data = &signal->data[start_idx];
        windowed_signal.length = window_size;
        windowed_signal.sample_rate = signal->sample_rate;
        windowed_signal.duration = (double)window_size / signal->sample_rate;
        windowed_signal.type = signal->type;
        snprintf(windowed_signal.name, sizeof(windowed_signal.name), 
                "Window %d", w);
        
        // Compute and display FFT
        FFTResult *fft = compute_fft(&windowed_signal);
        if (fft) {
            double window_time = start_idx / signal->sample_rate;
            printf("Window %d (t=%.3fs):\n", w, window_time);
            
            // Show dominant frequencies
            int half_length = fft->length / 2;
            double max_mag = 0.0;
            int max_idx = 0;
            
            for (int i = 1; i < half_length && i < 20; i++) {
                if (fft->magnitude[i] > max_mag) {
                    max_mag = fft->magnitude[i];
                    max_idx = i;
                }
            }
            
            if (max_mag > 0) {
                printf("  Dominant frequency: %.1f Hz (magnitude: %.4f)\n", 
                       fft->frequency[max_idx], max_mag);
            }
            
            free_fft_result(fft);
        }
    }
    printf("\n");
}