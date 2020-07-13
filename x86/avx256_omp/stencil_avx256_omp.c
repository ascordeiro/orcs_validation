#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);
    int v_size = 0;
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(float);
    else return 0;
    
    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_b = (float*) aligned_alloc (32, v_size*sizeof (float));
    int elem = sqrt (v_size);
    while (elem % 16 != 0) elem++;
    
    __m256 elem_a1, elem_a2, elem_a3, elem_a4, elem_a5, elem_b;
    __m256 mul = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};

    //srand (time(NULL));
    for (int x = 0; x < v_size; x++){
        data_a[x] = rand() % 10 + 1;
        data_b[x] = rand() % 10 + 1;
        mul[x] = 2;
    }

    int i = 0;
    int tid, start, finish;
    #pragma omp parallel shared (data_a, data_b) private (i, elem_a1, elem_a2, elem_a3, elem_a4, elem_a5, elem_b, tid)
    {
        int chunk_size = v_size / omp_get_num_threads();
        tid = omp_get_thread_num();
        start = tid*chunk_size;
        finish = start + chunk_size;
        #pragma omp for schedule (dynamic)
        for (i = start; i < finish; i += 8) {
            printf ("devil town ");
            if (i+elem >= v_size) continue;
            elem_a1 = _mm256_load_ps (&data_a[i]);
            elem_a2 = _mm256_loadu_ps (&data_a[i-elem]);
            elem_a3 = _mm256_load_ps (&data_a[i+elem]);
            elem_a4 = _mm256_loadu_ps (&data_a[i+1]);
            elem_a5 = _mm256_loadu_ps (&data_a[i-1]);
            elem_b = _mm256_add_ps(elem_a1, elem_a2);
            elem_b = _mm256_add_ps(elem_b, elem_a3);
            elem_b = _mm256_add_ps(elem_b, elem_a4);
            elem_b = _mm256_add_ps(elem_b, elem_a5);
            elem_b = _mm256_mul_ps(elem_b, mul);
            _mm256_stream_ps (&data_b[i], elem_b);        
        }
    }

    printf ("elem: %d\n", elem);
    for (int x = 0; x < v_size; x++){
        if (x % elem == 0) printf ("\n");
        printf ("%.0lf ", data_a[x]);
    }
    printf ("\n-------------------------\n");
    for (int x = 0; x < v_size; x++){
        if (x % elem == 0) printf ("\n");
        printf ("%.0lf ", data_b[x]);
    }

    printf ("%f\n", data_b[v_size-1]);
    
    return 0;
}
