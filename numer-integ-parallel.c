#include <stdio.h>
#include <math.h>
#include <omp.h>

// Function to integrate: f(x) = x^2
double f(double x) {
    return x * x;
}

// Trapezoidal Rule with OpenMP
double trapezoidal_integral_parallel(double a, double b, int n, int num_threads) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;

    // Parallelizing the loop with reduction for summation
    #pragma omp parallel for num_threads(num_threads) reduction(+:sum)
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }

    return sum * h;
}

int main() {
    double a = 0.0, b = 10.0;
    int n = 1000000; // Number of intervals
    double result, start, end, execution_time;
    double total_time;
    
    // Thread configurations to test
    int thread_counts[] = {1, 4, 8};

    // Loop over different thread counts
    for (int t = 0; t < 3; t++) {
        int num_threads = thread_counts[t];
        total_time = 0.0; // Reset total time for averaging
        
        printf("\nRunning with %d threads:\n", num_threads);
        
        // Run 10 times to get the average execution time
        for (int i = 0; i < 10; i++) {
            start = omp_get_wtime();
            result = trapezoidal_integral_parallel(a, b, n, num_threads);
            end = omp_get_wtime();
            
            execution_time = end - start;
            total_time += execution_time;

            printf("Run %d: Integral Approximation = %lf, Execution Time = %lf seconds\n", i + 1, result, execution_time);
        }

        double average_time = total_time / 10.0;
        printf("\nAverage Execution Time with %d threads: %lf seconds\n", num_threads, average_time);
    }

    return 0;
}
