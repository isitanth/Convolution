#include "../include/convolution.h"
#include <time.h>

// Function prototypes for additional visualization functions
void plot_signal_detailed(const Signal *signal);
void plot_signals_comparison(const Signal *sig1, const Signal *sig2, const char *title);
void show_signal_spectrogram(const Signal *signal, int window_size);

// Interactive menu functions
void show_main_menu(void);
void demo_basic_convolution(void);
void demo_signal_filtering(void);
void demo_frequency_analysis(void);
void demo_system_impulse_response(void);
void demo_custom_signals(void);
void demo_performance_comparison(void);

// Utility function to get user input
int get_user_choice(int min, int max) {
    int choice;
    while (1) {
        printf("Enter your choice (%d-%d): ", min, max);
        if (scanf("%d", &choice) == 1 && choice >= min && choice <= max) {
            return choice;
        } else {
            printf("Invalid choice. Please try again.\n");
            // Clear input buffer
            while (getchar() != '\n');
        }
    }
}

// Main function
int main() {
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║              CONVOLUTION EXPLORER              ║\n");
    printf("║     Understanding Signal Processing through    ║\n");
    printf("║        Visual and Mathematical Analysis        ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    printf("Welcome! This interactive application demonstrates how convolution\n");
    printf("works in both time and frequency domains.\n\n");
    
    printf("What is Convolution?\n");
    printf("Convolution is a mathematical operation that combines two signals\n");
    printf("to produce a third signal. It's fundamental to:\n");
    printf("• Digital signal processing\n");
    printf("• Image processing\n");
    printf("• System analysis\n");
    printf("• Filter design\n");
    printf("• Neural networks\n\n");
    
    init_visualization(800, 600);
    
    int choice;
    do {
        show_main_menu();
        choice = get_user_choice(0, 7);
        
        switch (choice) {
            case 1:
                demo_basic_convolution();
                break;
            case 2:
                demo_signal_filtering();
                break;
            case 3:
                demo_frequency_analysis();
                break;
            case 4:
                demo_system_impulse_response();
                break;
            case 5:
                demo_custom_signals();
                break;
            case 6:
                demo_performance_comparison();
                break;
            case 7:
                run_interactive_demo();
                break;
            case 0:
                printf("Thank you for using Convolution Explorer!\n");
                break;
        }
        
        if (choice != 0) {
            printf("\nPress Enter to continue...");
            getchar(); // consume newline
            getchar(); // wait for enter
        }
        
    } while (choice != 0);
    
    cleanup_visualization();
    return 0;
}

void show_main_menu(void) {
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("                        MAIN MENU\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("1. Basic Convolution Demo (Linear vs Circular)\n");
    printf("2. Signal Filtering Examples\n");
    printf("3. Frequency Domain Analysis\n");
    printf("4. System Impulse Response\n");
    printf("5. Custom Signal Generator\n");
    printf("6. Performance Comparison (Direct vs FFT)\n");
    printf("7. Interactive Tutorial\n");
    printf("0. Exit\n");
    printf("═══════════════════════════════════════════════════════════\n");
}

void demo_basic_convolution(void) {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║            BASIC CONVOLUTION DEMO             ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    printf("\nThis demo shows the difference between linear and circular convolution.\n");
    printf("We'll convolve a sine wave with a simple rectangular pulse.\n\n");
    
    // Create input signals
    double sample_rate = 1000.0; // 1 kHz
    double duration = 1.0;       // 1 second
    
    // Create a sine wave (5 Hz)
    Signal *sine = generate_sine_wave(5.0, 1.0, 0.0, duration, sample_rate);
    
    // Create a rectangular pulse (kernel)
    Signal *pulse = create_signal(50, sample_rate); // 50 sample pulse
    for (int i = 0; i < pulse->length; i++) {
        pulse->data[i] = (i < 25) ? 0.2 : 0.0; // 25-sample wide pulse
    }
    strcpy(pulse->name, "Rectangular Pulse");
    pulse->type = SIGNAL_CUSTOM;
    
    // Perform convolutions
    Signal *linear_conv = convolve(sine, pulse);
    Signal *circular_conv = convolve_circular(sine, pulse);
    
    // Display results
    plot_convolution_demo(sine, pulse, linear_conv);
    
    printf("\n--- Linear vs Circular Convolution Comparison ---\n");
    plot_signals_comparison(linear_conv, circular_conv, 
                           "Linear vs Circular Convolution");
    
    printf("Key Differences:\n");
    printf("• Linear convolution: Output length = %d + %d - 1 = %d samples\n", 
           sine->length, pulse->length, linear_conv->length);
    printf("• Circular convolution: Output length = max(%d, %d) = %d samples\n", 
           sine->length, pulse->length, circular_conv->length);
    printf("• Linear convolution assumes zero-padding\n");
    printf("• Circular convolution wraps around\n\n");
    
    // Cleanup
    free_signal(sine);
    free_signal(pulse);
    free_signal(linear_conv);
    free_signal(circular_conv);
}

void demo_signal_filtering(void) {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║             SIGNAL FILTERING DEMO             ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    printf("This demo shows how convolution is used for filtering signals.\n");
    printf("We'll create a noisy signal and apply different filters.\n\n");
    
    double sample_rate = 2000.0; // 2 kHz
    double duration = 0.5;       // 0.5 seconds
    
    // Create a composite signal (multiple frequencies)
    Signal *signal = generate_sine_wave(50.0, 0.8, 0.0, duration, sample_rate);
    Signal *high_freq = generate_sine_wave(300.0, 0.3, 0.0, duration, sample_rate);
    Signal *noise = generate_noise(0.1, duration, sample_rate);
    
    // Combine signals
    for (int i = 0; i < signal->length; i++) {
        signal->data[i] += high_freq->data[i] + noise->data[i];
    }
    strcpy(signal->name, "Noisy Composite Signal (50Hz + 300Hz + Noise)");
    
    printf("Original noisy signal:\n");
    plot_signal_detailed(signal);
    
    // Create simple low-pass filter (moving average)
    int filter_length = 20;
    Signal *lpf = create_signal(filter_length, sample_rate);
    for (int i = 0; i < filter_length; i++) {
        lpf->data[i] = 1.0 / filter_length; // Normalized moving average
    }
    strcpy(lpf->name, "Low-pass Filter (Moving Average)");
    
    // Apply filter
    Signal *filtered = convolve(signal, lpf);
    strcpy(filtered->name, "Filtered Signal");
    
    printf("\n--- Filtering Results ---\n");
    plot_signals_comparison(signal, filtered, "Before vs After Filtering");
    
    printf("\nObservations:\n");
    printf("• The low-pass filter attenuated high-frequency components\n");
    printf("• The 50Hz component is preserved\n");
    printf("• The 300Hz component and noise are reduced\n");
    printf("• This demonstrates convolution as filtering\n\n");
    
    // Cleanup
    free_signal(signal);
    free_signal(high_freq);
    free_signal(noise);
    free_signal(lpf);
    free_signal(filtered);
}

// Simplified versions of other demo functions for compilation
void demo_frequency_analysis(void) {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║           FREQUENCY DOMAIN ANALYSIS           ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    printf("This demo explores the frequency domain representation of convolution.\n");
    printf("Key concept: Convolution in time ↔ Multiplication in frequency\n\n");
    
    double sample_rate = 1000.0;
    double duration = 1.0;
    
    Signal *sig1 = generate_sine_wave(10.0, 1.0, 0.0, duration, sample_rate);
    Signal *sig2 = generate_gaussian_pulse(1.0, 0.05, 0.5, duration, sample_rate);
    
    Signal *conv_result = convolve(sig1, sig2);
    Signal *fft_conv_result = convolve_fft(sig1, sig2);
    
    plot_signal(sig1, "10 Hz Sine Wave", 1);
    plot_signal(sig2, "Gaussian Pulse", 2);
    plot_signals_comparison(conv_result, fft_conv_result, 
                           "Direct vs FFT Convolution");
    
    free_signal(sig1);
    free_signal(sig2);
    free_signal(conv_result);
    free_signal(fft_conv_result);
}

void demo_system_impulse_response(void) {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║           SYSTEM IMPULSE RESPONSE             ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    printf("This demo shows how convolution characterizes LTI systems.\n\n");
    
    double sample_rate = 1000.0;
    double duration = 1.0;
    
    Signal *step_input = create_signal((int)(duration * sample_rate), sample_rate);
    for (int i = 0; i < step_input->length; i++) {
        step_input->data[i] = (i > step_input->length / 4) ? 1.0 : 0.0;
    }
    strcpy(step_input->name, "Step Input");
    
    Signal *impulse_response = create_signal(200, sample_rate);
    double tau = 50.0;
    for (int i = 0; i < impulse_response->length; i++) {
        impulse_response->data[i] = exp(-i/tau) * (i > 0 ? 1.0/tau : 1.0/tau);
    }
    strcpy(impulse_response->name, "System Impulse Response");
    
    Signal *output = convolve(step_input, impulse_response);
    
    plot_signal(step_input, "Step Input", 1);
    plot_signal(impulse_response, "Impulse Response", 2);
    plot_signal(output, "System Output", 3);
    
    free_signal(step_input);
    free_signal(impulse_response);
    free_signal(output);
}

void demo_custom_signals(void) {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║            CUSTOM SIGNAL GENERATOR            ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    printf("Create your own signals and explore convolution!\n\n");
    
    printf("Available signal types:\n");
    printf("1. Sine wave\n2. Square wave\n3. Triangle wave\n");
    printf("4. Sawtooth wave\n5. White noise\n6. Gaussian pulse\n7. Impulse\n\n");
    
    printf("Choose first signal type: ");
    int type1 = get_user_choice(1, 7);
    printf("Choose second signal type: ");
    int type2 = get_user_choice(1, 7);
    
    double sample_rate = 1000.0, duration = 0.5;
    Signal *sig1 = NULL, *sig2 = NULL;
    
    switch (type1) {
        case 1: sig1 = generate_sine_wave(20.0, 1.0, 0.0, duration, sample_rate); break;
        case 2: sig1 = generate_square_wave(15.0, 1.0, duration, sample_rate); break;
        case 3: sig1 = generate_triangle_wave(25.0, 1.0, duration, sample_rate); break;
        case 4: sig1 = generate_sawtooth_wave(10.0, 1.0, duration, sample_rate); break;
        case 5: sig1 = generate_noise(0.5, duration, sample_rate); break;
        case 6: sig1 = generate_gaussian_pulse(1.0, 0.02, duration/2, duration, sample_rate); break;
        case 7: sig1 = generate_impulse(1.0, duration/4, duration, sample_rate); break;
    }
    
    switch (type2) {
        case 1: sig2 = generate_sine_wave(30.0, 0.8, 0.0, duration/2, sample_rate); break;
        case 2: sig2 = generate_square_wave(20.0, 0.8, duration/2, sample_rate); break;
        case 3: sig2 = generate_triangle_wave(35.0, 0.8, duration/2, sample_rate); break;
        case 4: sig2 = generate_sawtooth_wave(25.0, 0.8, duration/2, sample_rate); break;
        case 5: sig2 = generate_noise(0.3, duration/2, sample_rate); break;
        case 6: sig2 = generate_gaussian_pulse(0.8, 0.01, duration/4, duration/2, sample_rate); break;
        case 7: sig2 = generate_impulse(0.8, duration/8, duration/2, sample_rate); break;
    }
    
    if (sig1 && sig2) {
        Signal *result = convolve(sig1, sig2);
        plot_convolution_demo(sig1, sig2, result);
        
        free_signal(sig1);
        free_signal(sig2);
        free_signal(result);
    }
}

void demo_performance_comparison(void) {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║           PERFORMANCE COMPARISON              ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    printf("Comparing direct vs FFT convolution performance:\n\n");
    
    int lengths[] = {128, 256, 512, 1024};
    int num_tests = 4;
    double sample_rate = 1000.0;
    
    printf("%-10s %-15s %-15s %-15s\n", "Length", "Direct (ms)", "FFT (ms)", "Speedup");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < num_tests; i++) {
        int N = lengths[i];
        
        Signal *sig1 = generate_sine_wave(50.0, 1.0, 0.0, (double)N/sample_rate, sample_rate);
        Signal *sig2 = generate_gaussian_pulse(1.0, 0.01, 0.5, (double)N/sample_rate, sample_rate);
        
        clock_t start = clock();
        Signal *direct_result = convolve(sig1, sig2);
        clock_t direct_time = clock() - start;
        
        start = clock();
        Signal *fft_result = convolve_fft(sig1, sig2);
        clock_t fft_time = clock() - start;
        
        double direct_ms = ((double)direct_time / CLOCKS_PER_SEC) * 1000.0;
        double fft_ms = ((double)fft_time / CLOCKS_PER_SEC) * 1000.0;
        double speedup = (fft_ms > 0) ? direct_ms / fft_ms : 0.0;
        
        printf("%-10d %-15.3f %-15.3f %-15.2fx\n", N, direct_ms, fft_ms, speedup);
        
        free_signal(sig1);
        free_signal(sig2);
        if (direct_result) free_signal(direct_result);
        if (fft_result) free_signal(fft_result);
    }
}

void run_interactive_demo(void) {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║             INTERACTIVE TUTORIAL             ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    printf("Welcome to the interactive convolution tutorial!\n\n");
    printf("The discrete convolution formula is:\n");
    printf("    (x * h)[n] = Σ x[k] × h[n-k]\n\n");
    
    printf("Let's see this with simple signals:\n");
    
    Signal *x = create_signal(5, 1.0);
    double x_vals[] = {1, 2, 1, 0, 0};
    memcpy(x->data, x_vals, 5 * sizeof(double));
    strcpy(x->name, "Signal x[n] = [1, 2, 1, 0, 0]");
    
    Signal *h = create_signal(3, 1.0);
    double h_vals[] = {1, 0.5, 0.25};
    memcpy(h->data, h_vals, 3 * sizeof(double));
    strcpy(h->name, "Signal h[n] = [1, 0.5, 0.25]");
    
    Signal *y = convolve(x, h);
    
    plot_convolution_demo(x, h, y);
    
    printf("Manual calculation:\n");
    printf("y[0] = x[0]×h[0] = 1×1 = 1\n");
    printf("y[1] = x[0]×h[1] + x[1]×h[0] = 1×0.5 + 2×1 = 2.5\n");
    printf("y[2] = x[0]×h[2] + x[1]×h[1] + x[2]×h[0] = 1×0.25 + 2×0.5 + 1×1 = 2.25\n\n");
    
    printf("Key Takeaways:\n");
    printf("• Convolution combines two signals to create a third\n");
    printf("• Time domain convolution = Frequency domain multiplication\n");
    printf("• FFT makes convolution faster for large signals\n");
    printf("• Convolution is fundamental to signal processing\n\n");
    
    free_signal(x);
    free_signal(h);
    free_signal(y);
}