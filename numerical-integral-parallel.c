#include <stdio.h>
#include <omp.h>

// Optimized function for parallel numerical integration
double trapezoidal_integral_parallel(double a, double b, int n, int num_threads, int is_dynamic) {
    double h = (b - a) / n;
    double integral = 0.0;
    
    // Optimized chunk size formula
    int chunk_size = n / (num_threads * 5);

    // OpenMP scheduling ka behtareen setup
    if (is_dynamic) {
        #pragma omp parallel for num_threads(num_threads) schedule(dynamic, chunk_size) reduction(+:integral)
        for (int i = 0; i < n; i++) {  
            double x = a + (i + 0.5) * h;  
            integral += (4.0 / (1 + x * x)); 
        }
    } else {
        #pragma omp parallel for num_threads(num_threads) schedule(static, chunk_size) reduction(+:integral)
        for (int i = 0; i < n; i++) {  
            double x = a + (i + 0.5) * h;  
            integral += (4.0 / (1 + x * x)); 
        }
    }

    integral *= h;
    return integral;
}

int main() {
    double a = 0.0, b = 1.0;
    int n = 100000000;
    int thread_counts[] = {2, 4, 8};  

    double total_time, execution_time, result;
    double start, end;

    // Threads ko CPU cores per bind karne ke liye:
    omp_set_dynamic(0);  
    omp_set_nested(1);

    for (int mode = 0; mode < 2; mode++) {
        printf("\n===== %s Scheduling =====\n", mode == 0 ? "Static" : "Dynamic");

        for (int t = 0; t < 3; t++) {
            int num_threads = thread_counts[t];
            total_time = 0.0;

            printf("\nRunning with %d threads:\n", num_threads);

            for (int i = 0; i < 10; i++) {
                start = omp_get_wtime();
                result = trapezoidal_integral_parallel(a, b, n, num_threads, mode);
                end = omp_get_wtime();

                execution_time = end - start;
                total_time += execution_time;

                printf("Run %d: Integral Approximation = %lf, Execution Time = %lf seconds\n", 
                       i + 1, result, execution_time);
            }

            double average_time = total_time / 10.0;
            printf("\nAverage Execution Time with %d threads: %lf seconds\n", num_threads, average_time);
        }
    }

    return 0;
}
