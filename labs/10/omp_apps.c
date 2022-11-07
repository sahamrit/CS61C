#include "omp_apps.h"
/* -------------------------------Utilties, Do Not Modify------------------------------*/
double *gen_array(int n)
{
    double *array = (double *)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++)
        array[i] = drand48();
    return array;
}

int verify(double *x, double *y, void (*funct)(double *x, double *y, double *z))
{
    double *z_v_add = (double *)malloc(ARRAY_SIZE * sizeof(double));
    double *z_oracle = (double *)malloc(ARRAY_SIZE * sizeof(double));
    (*funct)(x, y, z_v_add);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        z_oracle[i] = x[i] + y[i];
    }
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (z_oracle[i] != z_v_add[i])
            return 0;
    }
    return 1;
}

/* -------------------------------Vector Addition------------------------------*/
void v_add_naive(double *x, double *y, double *z)
{
#pragma omp parallel
    {
        for (int i = 0; i < ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double *x, double *y, double *z)
{
#pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        for (int i = thread_id; i < ARRAY_SIZE; i += threads)
            z[i] = x[i] + y[i];
    }
}

// Chunks Method
void v_add_optimized_chunks(double *x, double *y, double *z)
{
#pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        int block_size = ARRAY_SIZE / threads;
        int start = block_size * thread_id;
        int end;
        if (thread_id == threads - 1)
        {
            end = ARRAY_SIZE;
        }
        else
        {
            end = start + block_size;
        }
        for (int i = start; i < end; i += 1)
            z[i] = x[i] + y[i];
    }
}
// Naive: 8 threads took 1.354845 seconds
// Optimized adjacent: 1 thread(s) took 0.845806 seconds
// Optimized adjacent: 2 thread(s) took 0.569746 seconds
// Optimized adjacent: 3 thread(s) took 0.458920 seconds
// Optimized adjacent: 4 thread(s) took 0.465844 seconds
// Optimized adjacent: 5 thread(s) took 0.440953 seconds
// Optimized adjacent: 6 thread(s) took 0.439444 seconds
// Optimized adjacent: 7 thread(s) took 0.434970 seconds
// Optimized adjacent: 8 thread(s) took 0.491253 seconds
// Optimized chunks: 1 thread(s) took 0.843132 seconds
// Optimized chunks: 2 thread(s) took 0.477364 seconds
// Optimized chunks: 3 thread(s) took 0.397164 seconds
// Optimized chunks: 4 thread(s) took 0.373059 seconds
// Optimized chunks: 5 thread(s) took 0.390680 seconds
// Optimized chunks: 6 thread(s) took 0.367525 seconds
// Optimized chunks: 7 thread(s) took 0.367013 seconds
// Optimized chunks: 8 thread(s) took 0.374519 seconds
// Congrats! All vector tests passed!
/* -------------------------------Dot Product------------------------------*/
double dotp_naive(double *x, double *y, int arr_size)
{
    double global_sum = 0.0;
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < arr_size; i++)
#pragma omp critical
            global_sum += x[i] * y[i];
    }
    return global_sum;
}

// Manual Reduction
double dotp_manual_optimized(double *x, double *y, int arr_size)
{
    double global_sum = 0.0;
#pragma omp parallel
    {
        double local_sum = 0;
#pragma omp for
        for (int i = 0; i < arr_size; i++)
            local_sum += x[i] * y[i];
#pragma omp critical
        global_sum += local_sum;
    }
    return global_sum;
}
// Naive: 1 thread took 4.358808 seconds
// Manual Optimized: 1 thread(s) took 0.833744 seconds
// Manual Optimized: 2 thread(s) took 0.442590 seconds
// Manual Optimized: 3 thread(s) took 0.306795 seconds
// Manual Optimized: 4 thread(s) took 0.243372 seconds
// Manual Optimized: 5 thread(s) took 0.251273 seconds
// Manual Optimized: 6 thread(s) took 0.243337 seconds
// Manual Optimized: 7 thread(s) took 0.218838 seconds
// Manual Optimized: 8 thread(s) took 0.229458 seconds
// Reduction Optimized: 1 thread(s) took 0.843711 seconds
// Reduction Optimized: 2 thread(s) took 0.442290 seconds
// Reduction Optimized: 3 thread(s) took 0.302640 seconds
// Reduction Optimized: 4 thread(s) took 0.288925 seconds
// Reduction Optimized: 5 thread(s) took 0.250923 seconds
// Reduction Optimized: 6 thread(s) took 0.242683 seconds
// Reduction Optimized: 7 thread(s) took 0.219684 seconds
// Reduction Optimized: 8 thread(s) took 0.231989 seconds
// Congrats! All dotp tests passed
// Reduction Keyword
double dotp_reduction_optimized(double *x, double *y, int arr_size)
{
    double global_sum = 0.0;
#pragma omp parallel
    {
#pragma omp for reduction(+ \
                          : global_sum)
        for (int i = 0; i < arr_size; i++)
        {
            global_sum += x[i] * y[i];
        }
    }
    return global_sum;
}

char *compute_dotp(int arr_size)
{
    // Generate input vectors
    char *report_buf = (char *)malloc(BUF_SIZE), *pos = report_buf;
    double start_time, run_time;

    double *x = gen_array(arr_size), *y = gen_array(arr_size);
    double serial_result = 0.0, result = 0.0;

    // calculate result serially
    for (int i = 0; i < arr_size; i++)
    {
        serial_result += x[i] * y[i];
    }

    int num_threads = omp_get_max_threads();
    for (int i = 1; i <= num_threads; i++)
    {
        omp_set_num_threads(i);
        start_time = omp_get_wtime();
        for (int j = 0; j < REPEAT; j++)
            result = dotp_manual_optimized(x, y, arr_size);
        run_time = omp_get_wtime() - start_time;
        pos += sprintf(pos, "Manual Optimized: %d thread(s) took %f seconds\n", i, run_time);

        // verify result is correct (within some threshold)
        if (fabs(serial_result - result) > 0.001)
        {
            pos += sprintf(pos, "Incorrect result!\n");
            *pos = '\0';
            return report_buf;
        }
    }

    for (int i = 1; i <= num_threads; i++)
    {
        omp_set_num_threads(i);
        start_time = omp_get_wtime();

        for (int j = 0; j < REPEAT; j++)
        {
            result = dotp_reduction_optimized(x, y, arr_size);
        }

        run_time = omp_get_wtime() - start_time;
        pos += sprintf(pos, "Reduction Optimized: %d thread(s) took %f seconds\n",
                       i, run_time);

        // verify result is correct (within some threshold)
        if (fabs(serial_result - result) > 0.001)
        {
            pos += sprintf(pos, "Incorrect result!\n");
            *pos = '\0';
            return report_buf;
        }
    }

    // Only run this once because it's too slow..
    omp_set_num_threads(1);
    start_time = omp_get_wtime();
    for (int j = 0; j < REPEAT; j++)
        result = dotp_naive(x, y, arr_size);
    run_time = omp_get_wtime() - start_time;

    pos += sprintf(pos, "Naive: %d thread(s) took %f seconds\n", 1, run_time);
    *pos = '\0';
    return report_buf;
}