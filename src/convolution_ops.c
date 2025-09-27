#include "../include/convolution.h"

// Linear convolution (standard discrete convolution)
Signal* convolve(const Signal *signal1, const Signal *signal2) {
    if (!signal1 || !signal2) return NULL;
    
    // Output length for linear convolution
    int output_length = signal1->length + signal2->length - 1;
    double sample_rate = signal1->sample_rate; // Assume same sample rate
    
    Signal *result = create_signal(output_length, sample_rate);
    if (!result) return NULL;
    
    result->type = SIGNAL_CUSTOM;
    snprintf(result->name, sizeof(result->name), 
             "Conv(%s * %s)", signal1->name, signal2->name);
    
    // Perform convolution: y[n] = sum(x[k] * h[n-k]) for all valid k
    for (int n = 0; n < output_length; n++) {
        result->data[n] = 0.0;
        
        // Determine the range of k values for which both x[k] and h[n-k] are valid
        int k_min = (n >= signal2->length - 1) ? n - signal2->length + 1 : 0;
        int k_max = (n < signal1->length) ? n : signal1->length - 1;
        
        for (int k = k_min; k <= k_max; k++) {
            int h_index = n - k;
            if (h_index >= 0 && h_index < signal2->length && k >= 0 && k < signal1->length) {
                result->data[n] += signal1->data[k] * signal2->data[h_index];
            }
        }
    }
    
    return result;
}

// Circular convolution
Signal* convolve_circular(const Signal *signal1, const Signal *signal2) {
    if (!signal1 || !signal2) return NULL;
    
    // For circular convolution, both signals should have the same length
    // If they don't, we'll use the length of the longer signal and zero-pad
    int length = (signal1->length > signal2->length) ? signal1->length : signal2->length;
    double sample_rate = signal1->sample_rate;
    
    Signal *result = create_signal(length, sample_rate);
    if (!result) return NULL;
    
    result->type = SIGNAL_CUSTOM;
    snprintf(result->name, sizeof(result->name), 
             "CircConv(%s * %s)", signal1->name, signal2->name);
    
    // Perform circular convolution
    for (int n = 0; n < length; n++) {
        result->data[n] = 0.0;
        
        for (int k = 0; k < length; k++) {
            // Get signal1 value (zero-padded if necessary)
            double x_val = (k < signal1->length) ? signal1->data[k] : 0.0;
            
            // Get signal2 value with circular indexing (zero-padded if necessary)
            int h_index = (n - k + length) % length;
            double h_val = (h_index < signal2->length) ? signal2->data[h_index] : 0.0;
            
            result->data[n] += x_val * h_val;
        }
    }
    
    return result;
}

// Helper function to find next power of 2
static int next_power_of_2(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

// Fast Fourier Transform (Cooley-Tukey algorithm)
void fft_recursive(Complex *data, int n) {
    if (n <= 1) return;
    
    // Divide
    Complex *even = (Complex*)malloc(n/2 * sizeof(Complex));
    Complex *odd = (Complex*)malloc(n/2 * sizeof(Complex));
    
    for (int i = 0; i < n/2; i++) {
        even[i] = data[i*2];
        odd[i] = data[i*2 + 1];
    }
    
    // Conquer
    fft_recursive(even, n/2);
    fft_recursive(odd, n/2);
    
    // Combine
    for (int k = 0; k < n/2; k++) {
        double angle = -2.0 * PI * k / n;
        Complex twiddle = {cos(angle), sin(angle)};
        
        // Complex multiplication: twiddle * odd[k]
        Complex temp = {
            twiddle.real * odd[k].real - twiddle.imag * odd[k].imag,
            twiddle.real * odd[k].imag + twiddle.imag * odd[k].real
        };
        
        data[k].real = even[k].real + temp.real;
        data[k].imag = even[k].imag + temp.imag;
        
        data[k + n/2].real = even[k].real - temp.real;
        data[k + n/2].imag = even[k].imag - temp.imag;
    }
    
    free(even);
    free(odd);
}

// Inverse Fast Fourier Transform
void ifft_recursive(Complex *data, int n) {
    if (n <= 1) return;
    
    // Conjugate the input
    for (int i = 0; i < n; i++) {
        data[i].imag = -data[i].imag;
    }
    
    // Apply FFT
    fft_recursive(data, n);
    
    // Conjugate the output and scale
    for (int i = 0; i < n; i++) {
        data[i].real /= n;
        data[i].imag = -data[i].imag / n;
    }
}

// FFT-based convolution (faster for large signals)
Signal* convolve_fft(const Signal *signal1, const Signal *signal2) {
    if (!signal1 || !signal2) return NULL;
    
    // For FFT convolution, we need to zero-pad to avoid circular effects
    int conv_length = signal1->length + signal2->length - 1;
    int fft_size = next_power_of_2(conv_length);
    
    // Create FFT buffers
    Complex *fft1 = (Complex*)calloc(fft_size, sizeof(Complex));
    Complex *fft2 = (Complex*)calloc(fft_size, sizeof(Complex));
    
    if (!fft1 || !fft2) {
        free(fft1);
        free(fft2);
        return NULL;
    }
    
    // Copy signal data to FFT buffers (zero-padded)
    for (int i = 0; i < signal1->length; i++) {
        fft1[i].real = signal1->data[i];
        fft1[i].imag = 0.0;
    }
    
    for (int i = 0; i < signal2->length; i++) {
        fft2[i].real = signal2->data[i];
        fft2[i].imag = 0.0;
    }
    
    // Compute FFTs
    fft_recursive(fft1, fft_size);
    fft_recursive(fft2, fft_size);
    
    // Multiply in frequency domain (pointwise multiplication)
    for (int i = 0; i < fft_size; i++) {
        Complex temp = {
            fft1[i].real * fft2[i].real - fft1[i].imag * fft2[i].imag,
            fft1[i].real * fft2[i].imag + fft1[i].imag * fft2[i].real
        };
        fft1[i] = temp;
    }
    
    // Inverse FFT to get convolution result
    ifft_recursive(fft1, fft_size);
    
    // Create result signal
    Signal *result = create_signal(conv_length, signal1->sample_rate);
    if (!result) {
        free(fft1);
        free(fft2);
        return NULL;
    }
    
    result->type = SIGNAL_CUSTOM;
    snprintf(result->name, sizeof(result->name), 
             "FFTConv(%s * %s)", signal1->name, signal2->name);
    
    // Copy real part of IFFT result
    for (int i = 0; i < conv_length; i++) {
        result->data[i] = fft1[i].real;
    }
    
    free(fft1);
    free(fft2);
    
    return result;
}

// Compute FFT of a signal for frequency analysis
FFTResult* compute_fft(const Signal *signal) {
    if (!signal) return NULL;
    
    int fft_size = next_power_of_2(signal->length);
    
    FFTResult *result = (FFTResult*)malloc(sizeof(FFTResult));
    if (!result) return NULL;
    
    result->data = (Complex*)calloc(fft_size, sizeof(Complex));
    result->magnitude = (double*)malloc(fft_size * sizeof(double));
    result->phase = (double*)malloc(fft_size * sizeof(double));
    result->frequency = (double*)malloc(fft_size * sizeof(double));
    result->length = fft_size;
    
    if (!result->data || !result->magnitude || !result->phase || !result->frequency) {
        free_fft_result(result);
        return NULL;
    }
    
    // Copy signal data (zero-padded)
    for (int i = 0; i < signal->length; i++) {
        result->data[i].real = signal->data[i];
        result->data[i].imag = 0.0;
    }
    
    // Compute FFT
    fft_recursive(result->data, fft_size);
    
    // Compute magnitude, phase, and frequency arrays
    double freq_resolution = signal->sample_rate / fft_size;
    
    for (int i = 0; i < fft_size; i++) {
        // Magnitude spectrum
        result->magnitude[i] = sqrt(result->data[i].real * result->data[i].real + 
                                   result->data[i].imag * result->data[i].imag);
        
        // Phase spectrum
        result->phase[i] = atan2(result->data[i].imag, result->data[i].real);
        
        // Frequency bins
        if (i <= fft_size/2) {
            result->frequency[i] = i * freq_resolution;
        } else {
            result->frequency[i] = (i - fft_size) * freq_resolution;
        }
    }
    
    return result;
}

// Free FFT result memory
void free_fft_result(FFTResult *result) {
    if (result) {
        if (result->data) free(result->data);
        if (result->magnitude) free(result->magnitude);
        if (result->phase) free(result->phase);
        if (result->frequency) free(result->frequency);
        free(result);
    }
}

// Save signal to CSV file for external analysis
void save_signal_to_file(const Signal *signal, const char *filename) {
    if (!signal || !filename) return;
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }
    
    fprintf(file, "# %s\n", signal->name);
    fprintf(file, "# Sample Rate: %.1f Hz\n", signal->sample_rate);
    fprintf(file, "# Length: %d samples\n", signal->length);
    fprintf(file, "# Duration: %.6f seconds\n", signal->duration);
    fprintf(file, "Time,Amplitude\n");
    
    for (int i = 0; i < signal->length; i++) {
        double time = (double)i / signal->sample_rate;
        fprintf(file, "%.6f,%.6f\n", time, signal->data[i]);
    }
    
    fclose(file);
    printf("Signal saved to %s\n", filename);
}

// Load signal from CSV file
Signal* load_signal_from_file(const char *filename) {
    if (!filename) return NULL;
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s for reading\n", filename);
        return NULL;
    }
    
    char line[256];
    double sample_rate = 44100.0; // Default
    int estimated_length = 0;
    
    // First pass: read header and count data lines
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') {
            if (strstr(line, "Sample Rate:")) {
                sscanf(line, "# Sample Rate: %lf Hz", &sample_rate);
            }
        } else if (line[0] != 'T') { // Skip "Time,Amplitude" header
            estimated_length++;
        }
    }
    
    if (estimated_length == 0) {
        fclose(file);
        return NULL;
    }
    
    // Create signal
    Signal *signal = create_signal(estimated_length, sample_rate);
    if (!signal) {
        fclose(file);
        return NULL;
    }
    
    // Second pass: read data
    rewind(file);
    int data_index = 0;
    
    while (fgets(line, sizeof(line), file) && data_index < estimated_length) {
        if (line[0] != '#' && line[0] != 'T') {
            double time, amplitude;
            if (sscanf(line, "%lf,%lf", &time, &amplitude) == 2) {
                signal->data[data_index] = amplitude;
                data_index++;
            }
        }
    }
    
    signal->length = data_index;
    strcpy(signal->name, "Loaded from file");
    
    fclose(file);
    return signal;
}