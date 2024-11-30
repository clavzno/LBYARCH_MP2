#include <stdio.h>

double dot_product(double *A, double *B, int n) {
    double sdot = 0.0;
    for (int i = 0; i < n; ++i) {
        sdot += A[i] * B[i];
    }
    return sdot;
}

int main() {
    int n;
    double A[100], B[100]; // Assuming a maximum vector length of 100

    while (1) {
        printf("Enter the vector length (n): ");
        if (scanf("%d", &n) != 1 || n <= 0) {
            printf("Invalid input. Please enter a positive integer.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Enter the elements of vector A:\n");
        for (int i = 0; i < n; ++i) {
            if (scanf("%lf", &A[i]) != 1) {
                printf("Invalid input. Please enter a valid number.\n");
                while (getchar() != '\n'); // Clear input buffer
                break;
            }
        }

        printf("Enter the elements of vector B:\n");
        for (int i = 0; i < n; ++i) {
            if (scanf("%lf", &B[i]) != 1) {
                printf("Invalid input. Please enter a valid number.\n");
                while (getchar() != '\n'); // Clear input buffer
                break;
            }
        }

        double sdot = dot_product(A, B, n);
        printf("Dot product: %f\n", sdot);
    }

    return 0;
}