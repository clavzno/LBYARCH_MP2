#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> //memory alloc
// uses _aligned_malloc: useful for SIMD single instruction multiple data operations
// uses _aligned_free: frees the memory that was allocated w the above instruction.
#include <stdint.h>
#include <math.h>
#include <time.h>
// #include "timer.c" can't be used anymore lol

#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define RESET "\x1B[0m"

extern void dot_product(int size, double* vec1, double* vec2, double* result);

// user input initialization of vectors
void initialize_vectors_from_user(double* vec1, double* vec2, int size) {
    printf("Enter elements one at a time for each vector.\n");
    printf("Enter %d elements for vector 1:\n", size);
    for (int i = 0; i < size; i++) {
        printf("vec1[%d]: ", i);
        scanf("%lf", &vec1[i]);
        printf("\n");
    }

    printf("Enter %d elements for vector 2:\n", size);
    for (int i = 0; i < size; i++) {
        printf("vec2[%d]: ", i);
        scanf("%lf", &vec2[i]);
        printf("\n");
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
        clock_t start_time = clock(); // clock function returns processor time
        dot_func(size, vec1, vec2, result);
        clock_t end_time = clock();
        total_duration += (double)(end_time - start_time) / CLOCKS_PER_SEC; //add to overall duration
        // CLOCKS_PER_SEC: const that represents the clock ticks into seconds
    }
    return total_duration / 20.0;
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
    double result_c, sdot;

    if (!vec1 || !vec2) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    // collect user inputs for vectors
    initialize_vectors_from_user(vec1, vec2, size);

    // compute and verify results
    printf(RED"\nDot Product Validation (size = %d)\n" RESET, size);
    //calculate_dot_product_c(size, vec1, vec2, &result_c);
    //dot_product(size, vec1, vec2, &sdot);
    //printf("C Implementation: %.3f ---- Assembly Implementation: %.3f\n", result_c, sdot);

    printf("C Implementation:\n");
    double avg_time_c = compute_execution_time(calculate_dot_product_c, size, vec1, vec2, &result_c);
    printf("Time: %.6f seconds --- Dot Product: %.3f\n", avg_time_c, result_c);
    printf("Assembly Implementation:\n");
    double avg_time_asm = compute_execution_time((void (*)(int, double*, double*, double*))dot_product, size, vec1, vec2, &sdot);
    printf("Time: %.6f seconds --- Dot Product: %.3f\n", avg_time_asm, sdot);

    printf("C implementation vs ASM implmenetation time difference: ");
    printf("%.6f\n", avg_time_c-avg_time_asm);

    // checking of results if same or not
    printf(BLUE"Correctness: "RESET);
    if (fabs(result_c - sdot) < 1e-3) {
        printf("Results are consistent.\n");
    } else {
        printf("Results do not match.\n");
    }

        //time analysis
        printf(BLUE"Time comparison: "RESET);
        if (fabs(avg_time_asm < avg_time_c)) {
            printf("ASM Kernel runs runs faster than C Kernel.\n");
        } else if (fabs(avg_time_asm > avg_time_c)) {
            printf("C Kernel runs runs faster than ASM Kernel.\n");
        } else {
            // (fabs(avg_time_asm == avg_time_c))
            printf("Time difference is too close to be compared OR time is equal.");
        }

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

        // to allocate memory
        double *vec1 = (double*)_aligned_malloc(size * sizeof(double), 16);
        double *vec2 = (double*)_aligned_malloc(size * sizeof(double), 16);
        double result_c, sdot;

        if (!vec1 || !vec2) {
            fprintf(stderr, "Memory allocation failed.\n");
            return;
        }

        // randomly initialize vectors
        auto_initialize_vectors(vec1, vec2, size);

        printf(RED"\nLarge Vector Computation (size = %d)\n"RESET, size);
        printf("C Implementation:\n");
        double avg_time_c = compute_execution_time(calculate_dot_product_c, size, vec1, vec2, &result_c);
        printf("Time: %.6f seconds --- Dot Product: %.3f\n", avg_time_c, result_c);

        printf("Assembly Implementation:\n");
        double avg_time_asm = compute_execution_time((void (*)(int, double*, double*, double*))dot_product, size, vec1, vec2, &sdot);
        printf("Time: %.6f seconds --- Dot Product: %.3f\n", avg_time_asm, sdot);

        printf("C implementation vs ASM implmenetation time difference: ");
        printf("%.6f\n", avg_time_c-avg_time_asm);

       // checking of results if same or not
        printf(BLUE"Correctness: "RESET);
        if (fabs(result_c - sdot) < 1e-6) {
            //math.h: fabs computes absolute value
            //1e-6 is the tolerance level for comparision (10^{-6}). considers floating-point precision errors
            printf("Results are consistent.\n");
        } else {
            printf("Results do not match.\n");
        }

        //time analysis
        printf(BLUE"Time comparison: "RESET);
        if (fabs(avg_time_asm < avg_time_c)) {
            printf("ASM Kernel runs runs faster than C Kernel.\n");
        } else if (fabs(avg_time_asm > avg_time_c)) {
            printf("C Kernel runs runs faster than ASM Kernel.\n");
        } else {
            // (fabs(avg_time_asm == avg_time_c))
            printf("Time difference is too close to be compared OR time is equal.");
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
