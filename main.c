#include <stdio.h>
#include "timer.c"
extern int asmdot(int n, double *A, double *B);

double dot_product(double *A, double *B, int n)
{
    double sdot = 0.0;
    for (int i = 0; i < n; ++i)
    {
        sdot += A[i] * B[i];
    }
    return sdot;
}

long runtwenty( double *A, double *B, int n)
{
    long averageExecutionTime = 0;
    long start, end, executionTime;
    for (int i = 0; i < 20; ++i)
    {
        printf("ITERATION # %d\n", i);
        start = currentTimeMillis();
        printf("Start time: %ld\n", start);
        //run dot prod
        double sdot = dot_product(A, B, n);
        end = currentTimeMillis();
        printf("End time: %ld\n", end);
        printf("Dot product: %f\n", sdot);
        averageExecutionTime += executionTime; // add to overall average
        printf("Current Total Average Execution Time: %ld\n", averageExecutionTime);
    }
    
    return averageExecutionTime/20; //returns average execution time
}

int main()
{
    int n;
    double A[100], B[100]; // assume a max vector length of 100

    printf("Enter the vector length (n): ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        printf("Invalid input. Please enter a positive integer.\n");
        while (getchar() != '\n'); // Clear input buffer
    }

    printf("Enter the elements of vector A:\n");
    for (int i = 0; i < n; ++i)
    {
        if (scanf("%lf", &A[i]) != 1)
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear input buffer
            break;
        }
    }

    printf("Enter the elements of vector B:\n");
    for (int i = 0; i < n; ++i)
    {
        if (scanf("%lf", &B[i]) != 1)
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear input buffer
            break;
        }
    }

    double sdot = dot_product(A, B, n);
    printf("Dot product: %f\n", sdot);

    printf("RUNNING 20 TIMES...\n");
    long time = runtwenty(A, B, n);
    printf("Time taken: %ld ms\n", time);

    return 0;
}