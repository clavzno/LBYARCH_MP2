#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> //memory alloc
// uses _aligned_malloc: useful for SIMD single instruction multiple data operations
// uses _aligned_free: frees the memory that was allocated w the above instruction.
#include <stdint.h>
#include <math.h>
#include <time.h>

extern void dot_product(int size, double* vec1, double* vec2, double* result);

// user input initialization of vectors
void initialize_vectors_from_user(double* vec1, double* vec2, int size) {
    printf("Enter %d elements for vector 1:\n", size);
    for (int i = 0; i < size; i++) {
        printf("vec1[%d]: ", i);
        scanf("%lf", &vec1[i]);
    }

    printf("Enter %d elements for vector 2:\n", size);
    for (int i = 0; i < size; i++) {
        printf("vec2[%d]: ", i);
        scanf("%lf", &vec2[i]);
    }
}

// random initialization of large vectors
void auto_initialize_vectors(double* vec1, double* vec2, int size) {
    srand((unsigned)time(NULL)); //random generator
    for (int i = 0; i < size; i++) {
        vec1[i] = (double)rand() / RAND_MAX * 10.0; // random double in range [0.0, 10.0]
        vec2[i] = (double)rand() / RAND_MAX * 10.0;
    }
}

// dot product calculation for c
void calculate_dot_product_c(int size, double* vec1, double* vec2, double* result) {
    *result = 0.0;
    for (int i = 0; i < size; i++) {
        *result += vec1[i] * vec2[i];
    }
}

// measure execution time for a dot product calculation function
double compute_execution_time(void (*dot_func)(int, double*, double*, double*), int size, double* vec1, double* vec2, double* result) {
    double total_duration = 0.0;
    for (int i = 0; i < 20; i++) {
        clock_t start_time = clock();
        dot_func(size, vec1, vec2, result);
        clock_t end_time = clock();
        total_duration += (double)(end_time - start_time) / CLOCKS_PER_SEC;
        // CLOCKS_PER_SEC: const that represents the clock ticks into seconds
    }
    return total_duration / 20.0;
}

// compare execution times and determine faster implementation
void compare_execution_times(double avg_time_c, double avg_time_asm) {
    printf("Performance Comparison:\n");
    printf("C Implementation Average Time: %.6f seconds\n", avg_time_c);
    printf("Assembly Implementation Average Time: %.6f seconds\n", avg_time_asm);

    if (avg_time_c < avg_time_asm) {
        printf("C implementation is faster by %.6f seconds.\n", avg_time_asm - avg_time_c);
    } else if (avg_time_c > avg_time_asm) {
        printf("Assembly implementation is faster by %.6f seconds.\n", avg_time_c - avg_time_asm);
    } else {
        printf("Both implementations have the same execution time.\n");
    }
}

// test function for small-sized vectors
void test_small_vectors() {
    int size;
    printf("Enter the size of the vectors: ");
    scanf("%d", &size);

    if (size <= 0) {
        fprintf(stderr, "Invalid size. Please provide a positive integer.\n");
        return;
    }

    // allocate memory
    double *vec1 = (double*)_aligned_malloc(size * sizeof(double), 16);
    double *vec2 = (double*)_aligned_malloc(size * sizeof(double), 16);
    double result_c = 0.0, result_asm = 0.0;
    double avg_time_c = 0.0, avg_time_asm = 0.0;

    if (!vec1 || !vec2) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    // collect user inputs for vectors
    initialize_vectors_from_user(vec1, vec2, size);
    
    // Measure execution time for C implementation
    avg_time_c = compute_execution_time(calculate_dot_product_c, size, vec1, vec2, &result_c);

    // Measure execution time for Assembly implementation
    avg_time_asm = compute_execution_time((void (*)(int, double*, double*, double*))dot_product, size, vec1, vec2, &result_asm);

    // compute and verify results
    printf("\nDot Product Validation (size = %d)\n", size);
    calculate_dot_product_c(size, vec1, vec2, &result_c);
    dot_product(size, vec1, vec2, &result_asm);
    printf("C Implementation: %.3f ---- Assembly Implementation: %.3f\n", result_c, result_asm);

    // checking of results if same or not
    printf("Correctness: ");
    if (fabs(result_c - result_asm) < 1e-3) {
        printf("Results are consistent.\n");
    } else {
        printf("Results do not match.\n");
    }

	compare_execution_times(avg_time_c, avg_time_asm);
    // free allocated memory
    _aligned_free(vec1);
    _aligned_free(vec2);
}

// large vectors with random initialization
void test_large_vectors() {
    int vector_sizes[] = {1 << 20, 1 << 24, 1 << 28}; //2^20, 2^24, 2^28 -> since laptop cant do 2^30
    int count_sizes = sizeof(vector_sizes) / sizeof(vector_sizes[0]);

    for (int i = 0; i < count_sizes; i++) {
        int size = vector_sizes[i];

        // allocate memory
        double *vec1 = (double*)_aligned_malloc(size * sizeof(double), 16);
        double *vec2 = (double*)_aligned_malloc(size * sizeof(double), 16);
        double result_c, result_asm;

        if (!vec1 || !vec2) {
            fprintf(stderr, "Memory allocation failed.\n");
            return;
        }

        // randomly initialize vectors
        auto_initialize_vectors(vec1, vec2, size);

        printf("\nLarge Vector Computation (size = %d)\n", size);
        printf("C Implementation:\n");
        double avg_time_c = compute_execution_time(calculate_dot_product_c, size, vec1, vec2, &result_c);
        printf("Time: %.6f seconds --- Dot Product: %.3f\n", avg_time_c, result_c);

        printf("Assembly Implementation:\n");
        double avg_time_asm = compute_execution_time((void (*)(int, double*, double*, double*))dot_product, size, vec1, vec2, &result_asm);
        printf("Time: %.6f seconds --- Dot Product: %.3f\n", avg_time_asm, result_asm);

        // compare execution times
        compare_execution_times(avg_time_c, avg_time_asm);

        // checking of results if same or not
        printf("Correctness: ");
        if (fabs(result_c - result_asm) < 1e-3) {
            printf("Results are consistent.\n");
        } else {
            printf("Results do not match.\n");
        }

        // free allocated memory
        _aligned_free(vec1);
        _aligned_free(vec2);
    }
}

int main() {
    printf("\nSmall Vector Test - User Input\n");
    test_small_vectors();

    printf("\nLarge Vector Tests - Random Initialization\n");
    test_large_vectors();

    printf("\nCompleted\n");
    return 0;
}
