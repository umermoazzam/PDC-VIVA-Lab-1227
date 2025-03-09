#include <stdio.h>
#include <math.h>
#include <time.h>

double f(double x) {
    return x * x;
}

// Trapezoidal Rule (Sequential)
double trapezoidal_integral(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;

    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }

    return sum * h;
}

int main() {
    double a = 0.0, b = 10.0;
    int n = 1000000; 
    clock_t start, end;
    double total_time = 0.0, execution_time;

    // Run the sequential version 10 times
    for (int i = 0; i < 10; i++) {
        start = clock();
        double result = trapezoidal_integral(a, b, n);
        end = clock();
                
        execution_time = (double)(end - start) / CLOCKS_PER_SEC;
        total_time += execution_time;

        printf("Run %d: Integral Approximation = %lf, Execution Time = %lf seconds\n", i + 1, result, execution_time);
    }

    double average_time = total_time / 10.0;
    printf("\nAverage Execution Time (Sequential): %lf seconds\n", average_time);

    return 0;
}
